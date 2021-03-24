// Copyright (C) 2019 Ryan Linnabary
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "collaborate/node.h"

#include <cstdint>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "collaborate/data_logger.h"
#include "collaborate/data_processor.h"
#include "collaborate/event_logger.h"
#include "collaborate/packet_forward.h"
#include "collaborate/packet_return.h"
#include "collaborate/platform.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/solar_panel.h"
#include "collaborate/subsystem_comm.h"
#include "collaborate/subsystem_power.h"
#include "collaborate/subsystem_sensing.h"

namespace osse {
namespace collaborate {

Node::Node(const std::string& _name,
           const uint16_t& _index,
           const uint16_t& _constellation,
           const Platform* _platform,
           const SubsystemComm& _comm_if,
           const SubsystemSensing& _sensing_if,
           const SubsystemPower& _subsystem_power,
           const SimulationClock* _clock,
           DataProcessor* _data_processor,
           EventLogger* _event_log,
           DataLogger* _data_log)
    : name_(_name),
      index_(_index),
      constellation_(_constellation),
      kPlatform_(_platform),
      kDataProcessor_(_data_processor),
      orbital_state_(kPlatform_->PredictOrbitalState(*_clock, 0)),
      comm_if_(_comm_if),
      sensing_if_(_sensing_if),
      mode_(kMode::Free),
      subsystem_power_(_subsystem_power),
      measurements_(std::vector<std::pair<uint64_t, uint16_t>>()),
      min_suggestions_(std::vector<Geodetic>()),
      max_suggestions_(std::vector<Geodetic>()),
      target_index_(std::numeric_limits<uint16_t>::max()),
      feedback_(std::vector<std::pair<bool, uint16_t>>()),
      num_neighbors_(0),
      clock_(_clock),
      event_log_(_event_log),
      log_buffer_({0, {}, {}, {}, {}, {}, {}, {}, {}, {}}),
      data_log_(_data_log),
      num_logs_(0) {
}

void Node::Update(const uint64_t& _offset_s,
                  const bool& _comm_orient,
                  const bool& _sensing_orient,
                  const bool& _measure,
                  const bool& _charge,
                  const bool& _power_update,
                  const bool& _communicate) {
  UpdateOrbitalState(_offset_s);
  if (_comm_orient) {
    UpdateCommAntenna();
  }
  if (_communicate) {
    UpdateCommunication();
  }
  if (_sensing_orient) {
    UpdateSensingAntenna();
  }
  if (_measure) {
    UpdateMeasurement();
  }
  if (_power_update) {
    UpdatePower(_charge);
  }
}

void Node::UpdateOrbitalState(const uint64_t& _offset_s) {
  kPlatform_->PredictOrbitalState(*clock_,
                                  _offset_s,
                                  &orbital_state_);
}

void Node::UpdateCommAntenna() {
  comm_if_.OrientAntenna(orbital_state_.orbit_frame(),
                         orbital_state_.body_frame());
}

void Node::UpdateSensingAntenna() {
  sensing_if_.OrientAntenna(orbital_state_.orbit_frame(),
                            orbital_state_.body_frame());
}

void Node::UpdateMeasurement() {
  for (auto plan = measurements_.begin(); plan != measurements_.end();) {
    if ((clock_->elapsed_s() > (*plan).first) && !sensing_if_.active()) {
      mode_ = kMode::Sensing;
      sensing_if_.Measure((*plan).second);
      event_log_->log()->info("[{}] N{} sensing \"{}\"",
                              *clock_,
                              index_,
                              sensing_if_.sensor()->kVariable());
      measurements_.erase(plan);
    } else {
      ++plan;
    }
  }
  sensing_if_.Update(*clock_, orbital_state_.position_m_rad());
  if (sensing_if_.complete()) {
    kDataProcessor_->Compute(ReadRawBuffer(sensing_if_.data_buffer()),
                             index_,
                             *clock_,
                             &min_suggestions_,
                             &max_suggestions_,
                             &feedback_);
    sensing_if_.set_complete(false);
    sensing_if_.EraseDataBuffer();
    mode_ = kMode::Carrying;
  }
}

void Node::UpdatePower(const bool& _charge) {
  double power_drain_w = 0;
  if (mode_ == kMode::Sensing) {
    power_drain_w += sensing_if_.sensor()->kPowerConsumedW();
  }
  if (comm_if_.mode() != SubsystemComm::kMode::Free) {
    power_drain_w += comm_if_.CalculatePowerDrainW();
  }
  subsystem_power_.Update(_charge,
                          *clock_,
                          orbital_state_.body_frame(),
                          orbital_state_.orbit_frame(),
                          power_drain_w,
                          orbital_state_.position_m_rad());
}

void Node::UpdateCommunication() {
  if (target_index_ != std::numeric_limits<uint16_t>::max()) {
    target_index_ = std::numeric_limits<uint16_t>::max();
  }
  if (comm_if_.mode() == SubsystemComm::kMode::Free) {
    target_index_ = comm_if_.Update(*clock_);
  }
}

void Node::PlanMeasurement(const uint64_t& _start_s,
                           const uint16_t& _return_index) {
  measurements_.push_back(std::make_pair(_start_s, _return_index));
}

void Node::AddressCommBuffer() {
  std::vector<uint8_t> buffer = comm_if_.data_buffer();
  if (buffer.size() == PacketForward::kPacketForwardSizeBytes) {
    PacketForward packet(comm_if_.data_buffer());
    comm_if_.EraseDataBuffer();
    std::vector<std::pair<uint16_t, uint64_t>> route;
    route = packet.DecodePartialRoute();
    bool found_self = false;
    uint16_t self_index = 0;
    uint16_t index = 0;
    while (index < route.size() && !found_self) {
      if (route[index].first == index_) {
        found_self = true;
        self_index = index;
      }
      ++index;
    }
    if (!found_self) {
      comm_if_.AddToStorage({route[0].first, route[0].second, packet});
    } else {
      if (self_index == route.size()-1) {
        PlanMeasurement(packet.event().second, packet.feedback());
      } else {
        comm_if_.AddToStorage({route[self_index+1].first,
                               route[self_index+1].second,
                               packet});
      }
    }
  } else if (buffer.size() == PacketReturn::kPacketReturnSizeBytes) {
    PacketReturn packet(comm_if_.data_buffer());
    comm_if_.EraseDataBuffer();
    std::vector<std::pair<uint16_t, uint64_t>> route;
    route = packet.DecodePartialRoute();
    bool found_self = false;
    uint16_t self_index = 0;
    uint16_t index = 0;
    while (index < route.size() && !found_self) {
      if (route[index].first == index_) {
        found_self = true;
        self_index = index;
      }
      ++index;
    }
    if (!found_self) {
      comm_if_.AddToStorage({route[0].first, route[0].second, packet});
    } else {
      if (self_index == route.size()-1) {
        kDataProcessor_->Regression(packet.success().first,
                  packet.success().second);
      } else {
        comm_if_.AddToStorage({route[self_index+1].first,
                               route[self_index+1].second,
                               packet});
      }
    }
  }
}

void Node::SwitchCommunication(const SubsystemComm::kMode& _mode) {
  comm_if_.set_mode(_mode);
}

void Node::BufferDataLog() {
  if (log_buffer_.counter == kLogBufferSize) {
    Flush();
    log_buffer_.counter = 0;
  }
  int counter = log_buffer_.counter;
  std::vector<double> geo = orbital_state_.ObtainGeodeticLog();
  log_buffer_.index[counter] = index_;
  log_buffer_.constellation[counter] = constellation_;
  uint64_t mode = 0;
  switch (mode_) {
  case(kMode::Free):
    mode = 0;
    break;
  case(kMode::Sensing):
    mode = 1;
    break;
  default:
    mode = 0;
    break;
  }
  if (comm_if_.mode() == SubsystemComm::kMode::Transmitting) {
    mode = 2;
  } else if (comm_if_.mode() == SubsystemComm::kMode::Receiving) {
    mode = 3;
  }
  log_buffer_.mode[counter] = mode;
  log_buffer_.latitude[counter] = geo[0];
  log_buffer_.longitude[counter] = geo[1];
  log_buffer_.altitude[counter] = geo[2];
  SolarPanel panel = subsystem_power_.solar_panels()[0];
  panel.Update(orbital_state_.body_frame(),
               orbital_state_.orbit_frame(),
               orbital_state_.position_m_rad());
  log_buffer_.energy[counter] = subsystem_power_.battery().energy_w_hr();
  log_buffer_.charging[counter] = subsystem_power_.charging();
  log_buffer_.area[counter] = panel.effective_area_m2();
  log_buffer_.num_neighbors[counter] = num_neighbors_;
  ++log_buffer_.counter;
}

void Node::Flush() {
  uint64_t count = log_buffer_.counter;
  uint64_t index = clock_->ticks() - count;
  data_log_->LogParameter(index_,
                          "index",
                          log_buffer_.index,
                          index,
                          count);
  data_log_->LogParameter(index_,
                          "constellation",
                          log_buffer_.constellation,
                          index,
                          count);
  data_log_->LogParameter(index_,
                          "mode",
                          log_buffer_.mode,
                          index,
                          count);
  data_log_->LogParameter(index_,
                          "latitude",
                          log_buffer_.latitude,
                          index,
                          count);
  data_log_->LogParameter(index_,
                          "longitude",
                          log_buffer_.longitude,
                          index,
                          count);
  data_log_->LogParameter(index_,
                          "altitude",
                          log_buffer_.altitude,
                          index,
                          count);
  data_log_->LogParameter(index_,
                          "energy",
                          log_buffer_.energy,
                          index,
                          count);
  data_log_->LogParameter(index_,
                          "charging",
                          log_buffer_.charging,
                          index,
                          count);
  data_log_->LogParameter(index_,
                          "area",
                          log_buffer_.area,
                          index,
                          count);
  data_log_->LogParameter(index_,
                          "num_neighbors",
                          log_buffer_.num_neighbors,
                          index,
                          count);
  num_logs_ += 1;
}

void Node::SetCommBuffer(std::vector<uint8_t> _comm_buffer) {
  comm_if_.set_data_buffer(_comm_buffer);
}

void Node::SetSensingBuffer(std::vector<uint8_t> _sensing_buffer) {
  sensing_if_.set_data_buffer(_sensing_buffer);
}

std::vector<uint8_t> Node::GetCommBuffer() {
  return comm_if_.data_buffer();
}

std::vector<uint8_t> Node::GetSensingBuffer() {
  return sensing_if_.data_buffer();
}

void Node::EraseCommBuffer() {
  comm_if_.EraseDataBuffer();
}

void Node::EraseSensingBuffer() {
  sensing_if_.EraseDataBuffer();
}

void Node::set_mode(const Node::kMode& _mode) {
  mode_ = _mode;
}

}  // namespace collaborate
}  // namespace osse
