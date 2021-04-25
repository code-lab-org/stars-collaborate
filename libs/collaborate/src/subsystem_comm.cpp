// Copyright (C) 2019 The Ohio State University
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

#include "collaborate/subsystem_comm.h"

#include <cstdint>
#include <limits>
#include <vector>

#include "collaborate/antenna.h"
#include "collaborate/modem.h"
#include "collaborate/reference_frame.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/util.h"

namespace osse {
namespace collaborate {

SubsystemComm::SubsystemComm(const Antenna* _antenna,
                             const Modem* _modem)
    : kModem_(_modem),
      storage_(std::vector<CommunicationEvent>()),
      mode_(kMode::Free),
      feedback_storage_(std::vector<FeedbackEvent>()),
      kAntenna_(_antenna),
      antenna_frame_(ReferenceFrame(kAntenna_->kRollRad(),
                                    kAntenna_->kPitchRad(),
                                    kAntenna_->kYawRad())),
      data_buffer_(std::vector<uint8_t>()),
      active_(false),
      elapsed_s_(0) {
}

uint16_t SubsystemComm::Update(const SimulationClock& _clock) {
  uint16_t target_index = std::numeric_limits<uint16_t>::max();
  auto event = storage_.begin();
  bool found = false;
  while (event != storage_.end() && !found) {
    if (_clock.elapsed_s() > (*event).elapsed_s_) {
      found = true;
      target_index = (*event).index_;
      data_buffer_ = (*event).packet_.payload();
      storage_.erase(event);
    } else {
      ++event;
    }
  }
  auto feedback = feedback_storage_.begin();
  while (feedback != feedback_storage_.end() && !found) {
    if (_clock.elapsed_s() > (*feedback).elapsed_s_) {
      found = true;
      target_index = (*feedback).index_;
      data_buffer_ = (*feedback).packet_.payload();
      feedback_storage_.erase(feedback);
    } else {
      ++feedback;
    }
  }
  return target_index;
}

uint64_t SubsystemComm::RequiredTransferDurationS() const {
  double tx_seconds = (data_buffer_.size()
                       * util::kBitsPerByte
                       / kModem_->kTxRateBitsPerS());
  double rx_seconds = (data_buffer_.size()
                       * util::kBitsPerByte
                       / kModem_->kRxRateBitsPerS());
  return std::fmax(tx_seconds, rx_seconds) + 3;
}

uint64_t SubsystemComm::RequiredTransferDurationS(
    const uint64_t& _buffer_size_bytes) const {
  double tx_seconds = (_buffer_size_bytes
                       * util::kBitsPerByte
                       / kModem_->kTxRateBitsPerS());
  double rx_seconds = (_buffer_size_bytes
                       * util::kBitsPerByte
                       / kModem_->kRxRateBitsPerS());
  return std::fmax(tx_seconds, rx_seconds) + 3;
}

double SubsystemComm::CalculatePowerDrainW() {
  double power_drain_w = 0;
  if (mode_ == kMode::Transmitting) {
    power_drain_w += kModem_->kTxConsumedPowerW();
  } else if (mode_ == kMode::Receiving) {
    power_drain_w += kModem_->kRxConsumedPowerW();
  }
  return power_drain_w;
}

void SubsystemComm::AddToStorage(SubsystemComm::CommunicationEvent _event) {
  storage_.push_back(_event);
}

void SubsystemComm::AddToStorage(
    SubsystemComm::FeedbackEvent _feedback_event) {
  feedback_storage_.push_back(_feedback_event);
}

void SubsystemComm::OrientAntenna(const ReferenceFrame& _orbit_frame,
                                  const ReferenceFrame& _body_frame) {
  antenna_frame_.Update(_orbit_frame, _body_frame);
}

void SubsystemComm::LoadData(const std::vector<uint8_t>& _payload) {
  for (auto byte : _payload) {
    data_buffer_.push_back(byte);
  }
}

void SubsystemComm::EraseDataBuffer() {
  std::vector<uint8_t> empty;
  data_buffer_ = empty;
}

void SubsystemComm::set_data_buffer(const std::vector<uint8_t>& _data_buffer) {
  data_buffer_ = _data_buffer;
}

void SubsystemComm::set_mode(const SubsystemComm::kMode& _mode) {
  mode_ = _mode;
}

}  // namespace collaborate
}  // namespace osse
