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

#include "collaborate/channel.h"

#include <cstdint>
#include <vector>

#include "collaborate/antenna.h"
#include "collaborate/attitude_matrix.h"
#include "collaborate/earth.h"
#include "collaborate/geodetic.h"
#include "collaborate/node.h"
#include "collaborate/orbital_state.h"
#include "collaborate/reference_frame.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/subsystem_comm.h"
#include "collaborate/util.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

Channel::Channel(Node* _tx_node, Node* _rx_node)
    : tx_node_(_tx_node),
      rx_node_(_rx_node),
      data_rate_bits_per_s_(CalculateDataRateBitsPerS()),
      omega_rad_per_s_(0),
      rx_power_w_(0),
      rx_gain_db_(0),
      rx_los_unit_(Vector()),
      tx_power_w_(0),
      tx_gain_db_(0),
      tx_los_unit_(Vector()),
      los_speed_m_per_s_(0),
      distance_m_(0),
      delay_s_(0),
      active_(false),
      fake_rx_buffer_bytes_(0),
      fake_tx_buffer_bytes_(0),
      log_buffer_(std::vector<double>()),
      error_flag_(false),
      success_flag_(false),
      open_(false),
      log_({}) {
}

void Channel::Start() {
  tx_node_->set_mode(Node::kMode::Carrying);
  rx_node_->set_mode(Node::kMode::Carrying);
  tx_node_->SwitchCommunication(SubsystemComm::kMode::Transmitting);
  rx_node_->SwitchCommunication(SubsystemComm::kMode::Receiving);
  fake_tx_buffer_bytes_ = PredictTransferSizeBytes();
  if (fake_tx_buffer_bytes_ == 0) {
    error_flag_ = true;
  }
  fake_rx_buffer_bytes_ = 0;
  active_ = true;
}

void Channel::Update(const SimulationClock& _clock, const bool& _flag) {
  Vector tx_position_m_rad = tx_node_->orbital_state().position_m_rad();
  Vector rx_position_m_rad = rx_node_->orbital_state().position_m_rad();
  if (_flag || earth::Visible(tx_position_m_rad, rx_position_m_rad)) {
    UpdateLosUnit();
    UpdateGainDb();
    UpdateOpen();
    UpdateDistanceM();
    UpdateLosSpeedMPerS();
    UpdateOmegaRadPerS();
    UpdateDelayS();
    UpdatePowerW();
    if (active_) {
      if (open_) {
        Buffer(_clock);
        FakeTransfer(_clock);
        if (fake_tx_buffer_bytes_ == 0) {
          success_flag_ = true;
          RealTransfer();
          Flush(_clock);
        }
      } else {
        error_flag_ = true;
        tx_node_->SwitchCommunication(SubsystemComm::kMode::Free);
        rx_node_->SwitchCommunication(SubsystemComm::kMode::Free);
      }
    }
  } else {
    open_ = false;
  }
}

uint64_t Channel::PredictTransferDurationS() const {
  return tx_node_->comm_if().RequiredTransferDurationS();
}

uint64_t Channel::PredictTransferSizeBytes() const {
  return tx_node_->comm_if().data_buffer().size();
}

void Channel::FakeTransfer(const SimulationClock& _clock) {
  uint64_t transferred_bytes;
  transferred_bytes = (1 + (_clock.last_increment_s()
                            * data_rate_bits_per_s_
                            / 8));
  if (fake_tx_buffer_bytes_ <= transferred_bytes) {
    transferred_bytes = fake_tx_buffer_bytes_;
  }
  fake_tx_buffer_bytes_ -= transferred_bytes;
  fake_rx_buffer_bytes_ += transferred_bytes;
}

void Channel::RealTransfer() {
  rx_node_->SetCommBuffer(tx_node_->comm_if().data_buffer());
  tx_node_->set_mode(Node::kMode::Free);
  rx_node_->set_mode(Node::kMode::Free);
  tx_node_->SwitchCommunication(SubsystemComm::kMode::Free);
  rx_node_->SwitchCommunication(SubsystemComm::kMode::Free);
}

void Channel::Buffer(const SimulationClock& _clock) {
  Geodetic tx_geodetic_rad_m = tx_node_->orbital_state().geodetic_rad_m();
  Geodetic rx_geodetic_rad_m = rx_node_->orbital_state().geodetic_rad_m();
  log_.ticks.push_back(_clock.ticks());
  log_.year.push_back(_clock.date_time().Year());
  log_.month.push_back(_clock.date_time().Month());
  log_.day.push_back(_clock.date_time().Day());
  log_.hour.push_back(_clock.date_time().Hour());
  log_.minute.push_back(_clock.date_time().Minute());
  log_.second.push_back(_clock.date_time().Second());
  log_.microsecond.push_back(_clock.date_time().Microsecond());
  log_.los_speed.push_back(los_speed_m_per_s_);
  log_.omega.push_back(omega_rad_per_s_);
  log_.distance.push_back(distance_m_);
  log_.delay.push_back(delay_s_);
  log_.data_rate.push_back(data_rate_bits_per_s_);
  log_.tx_idx.push_back(tx_node_->index());
  log_.tx_buffer.push_back(fake_tx_buffer_bytes_);
  log_.tx_lon.push_back(tx_geodetic_rad_m.longitude_rad());
  log_.tx_lat.push_back(tx_geodetic_rad_m.latitude_rad());
  log_.tx_alt.push_back(tx_geodetic_rad_m.altitude_m());
  log_.tx_gain.push_back(tx_gain_db_);
  log_.tx_power.push_back(tx_power_w_);
  log_.rx_idx.push_back(rx_node_->index());
  log_.rx_buffer.push_back(fake_rx_buffer_bytes_);
  log_.rx_lon.push_back(rx_geodetic_rad_m.longitude_rad());
  log_.rx_lat.push_back(rx_geodetic_rad_m.latitude_rad());
  log_.rx_alt.push_back(rx_geodetic_rad_m.altitude_m());
  log_.rx_gain.push_back(rx_gain_db_);
  log_.rx_power.push_back(rx_power_w_);
}

void Channel::Flush(const SimulationClock& _clock) {
  uint64_t length = log_.year.size();
  std::stringstream stream;
  std::stringstream time;
  stream << "output/"
         << std::fixed
         << std::setw(10)
         << std::setfill('0')
         << _clock.elapsed_s()
         << "s_N"
         << std::fixed
         << std::setw(5)
         << std::setfill('0')
         << tx_node_->index()
         << "_N"
         << std::fixed
         << std::setw(5)
         << std::setfill('0')
         << rx_node_->index()
         << "_channel.nc4";
  DataLogger log(stream.str());
  log.Channel(length);
  log.LogSeries("time", log_.ticks.data(), length);
  log.LogSeries("year", log_.year.data(), length);
  log.LogSeries("month", log_.month.data(), length);
  log.LogSeries("day", log_.day.data(), length);
  log.LogSeries("hour", log_.hour.data(), length);
  log.LogSeries("minute", log_.minute.data(), length);
  log.LogSeries("second", log_.second.data(), length);
  log.LogSeries("microsecond", log_.microsecond.data(), length);
  log.LogSeries("los_speed", log_.los_speed.data(), length);
  log.LogSeries("omega", log_.omega.data(), length);
  log.LogSeries("distance", log_.distance.data(), length);
  log.LogSeries("delay", log_.delay.data(), length);
  log.LogSeries("data_rate", log_.data_rate.data(), length);
  log.LogSeries("tx_idx", log_.tx_idx.data(), length);
  log.LogSeries("tx_buffer", log_.tx_buffer.data(), length);
  log.LogSeries("tx_lon", log_.tx_lon.data(), length);
  log.LogSeries("tx_lat", log_.tx_lat.data(), length);
  log.LogSeries("tx_alt", log_.tx_alt.data(), length);
  log.LogSeries("tx_gain", log_.tx_gain.data(), length);
  log.LogSeries("tx_power", log_.tx_power.data(), length);
  log.LogSeries("rx_idx", log_.rx_idx.data(), length);
  log.LogSeries("rx_buffer", log_.rx_buffer.data(), length);
  log.LogSeries("rx_lon", log_.rx_lon.data(), length);
  log.LogSeries("rx_lat", log_.rx_lat.data(), length);
  log.LogSeries("rx_alt", log_.rx_alt.data(), length);
  log.LogSeries("rx_gain", log_.rx_gain.data(), length);
  log.LogSeries("rx_power", log_.rx_power.data(), length);
  log_.ticks.clear();
  log_.year.clear();
  log_.month.clear();
  log_.day.clear();
  log_.hour.clear();
  log_.minute.clear();
  log_.second.clear();
  log_.microsecond.clear();
  log_.los_speed.clear();
  log_.omega.clear();
  log_.distance.clear();
  log_.delay.clear();
  log_.data_rate.clear();
  log_.tx_idx.clear();
  log_.tx_buffer.clear();
  log_.tx_lon.clear();
  log_.tx_lat.clear();
  log_.tx_alt.clear();
  log_.tx_gain.clear();
  log_.tx_power.clear();
  log_.rx_idx.clear();
  log_.rx_buffer.clear();
  log_.rx_lon.clear();
  log_.rx_lat.clear();
  log_.rx_alt.clear();
  log_.rx_gain.clear();
  log_.rx_power.clear();
}

void Channel::UpdateOmegaRadPerS() {
  double tx_omega_rad_per_s = tx_node_->comm_if().kModem()->kTxOmegaRadPerS();
  double rx_omega_rad_per_s = rx_node_->comm_if().kModem()->kRxOmegaRadPerS();
  double omega_rad_per_s = std::fmin(tx_omega_rad_per_s, rx_omega_rad_per_s);
  double factor = (1 + (los_speed_m_per_s_ / kSpeedOfLightMPerS));
  omega_rad_per_s_ = (factor * omega_rad_per_s);
}

double Channel::CalculateDataRateBitsPerS() const {
  double tx_rate_bits_per_s = tx_node_->comm_if().kModem()->kTxRateBitsPerS();
  double rx_rate_bits_per_s = rx_node_->comm_if().kModem()->kRxRateBitsPerS();
  return std::fmin(tx_rate_bits_per_s, rx_rate_bits_per_s);
}

void Channel::UpdateDistanceM() {
  Vector tx_position_m = tx_node_->orbital_state().position_m_rad();
  Vector rx_position_m = rx_node_->orbital_state().position_m_rad();
  Vector distance_m = (tx_position_m - rx_position_m);
  distance_m.CompleteCoordinates();
  distance_m_ = distance_m.r_m();
}

void Channel::UpdateLosUnit() {
  Vector tx_position_m = tx_node_->orbital_state().position_m_rad();
  Vector rx_position_m = rx_node_->orbital_state().position_m_rad();
  tx_los_unit_ = (rx_position_m - tx_position_m).Unit();
  rx_los_unit_ = (tx_position_m - rx_position_m).Unit();
}

void Channel::UpdateLosSpeedMPerS() {
  Vector tx_velocity_m_per_s = tx_node_->orbital_state().velocity_m_per_s();
  Vector rx_velocity_m_per_s = rx_node_->orbital_state().velocity_m_per_s();
  Vector difference_m_per_s = tx_velocity_m_per_s - rx_velocity_m_per_s;
  los_speed_m_per_s_ = difference_m_per_s.Dot(tx_los_unit_);
}

void Channel::UpdateGainDb() {
  Vector tx_pos = tx_node_->orbital_state().position_m_rad();
  Vector rx_pos = rx_node_->orbital_state().position_m_rad();
  if (!earth::Visible(rx_pos, tx_pos)) {
    tx_gain_db_ = 0;
    rx_gain_db_ = 0;
  } else {
    // Transmitter
    const Antenna* tx_antenna = tx_node_->comm_if().kAntenna();
    Vector tx_antenna_los = tx_los_unit_;
    OrbitalState tx_state = tx_node_->orbital_state();
    AttitudeMatrix tx_orbit_attitude = tx_state.orbit_frame().attitude();
    AttitudeMatrix tx_body_attitude = tx_state.body_frame().attitude();
    ReferenceFrame tx_antenna_frame = tx_node_->comm_if().antenna_frame();
    AttitudeMatrix tx_ant_attitude = tx_antenna_frame.attitude();
    tx_antenna_los = tx_orbit_attitude.InvertVector(tx_antenna_los);
    tx_antenna_los = tx_body_attitude.InvertVector(tx_antenna_los);
    tx_antenna_los = tx_ant_attitude.InvertVector(tx_antenna_los);
    tx_antenna_los.CompleteCoordinates();
    tx_gain_db_ = tx_antenna->GainDb(tx_antenna_los.theta_rad(),
                                     tx_antenna_los.phi_rad());
    // Receiver
    const Antenna* rx_antenna = rx_node_->comm_if().kAntenna();
    Vector rx_antenna_los = rx_los_unit_;
    OrbitalState rx_state = rx_node_->orbital_state();
    AttitudeMatrix rx_orbit_attitude = rx_state.orbit_frame().attitude();
    AttitudeMatrix rx_body_attitude = rx_state.body_frame().attitude();
    ReferenceFrame rx_antenna_frame = rx_node_->comm_if().antenna_frame();
    AttitudeMatrix rx_ant_attitude = rx_antenna_frame.attitude();
    rx_antenna_los = rx_orbit_attitude.InvertVector(rx_antenna_los);
    rx_antenna_los = rx_body_attitude.InvertVector(rx_antenna_los);
    rx_antenna_los = rx_ant_attitude.InvertVector(rx_antenna_los);
    rx_antenna_los.CompleteCoordinates();
    rx_gain_db_ = rx_antenna->GainDb(rx_antenna_los.theta_rad(),
                                     rx_antenna_los.phi_rad());
  }
}

void Channel::UpdateOpen() {
  constexpr double kMinGainDb = 0.0001;
  open_ = (tx_gain_db_ > kMinGainDb) && (rx_gain_db_ > kMinGainDb);
}

void Channel::UpdatePowerW() {
  tx_power_w_ = tx_node_->comm_if().kModem()->kTxRfPowerW();
  double lambda_m = (kSpeedOfLightMPerS
                     / (omega_rad_per_s_ / (util::kTwoPiRad)));
  double fslfs = std::pow(lambda_m / (4 * util::kPiRad * distance_m_),
                          2);
  rx_power_w_ = tx_power_w_ * tx_gain_db_ * rx_gain_db_ * fslfs;
}

void Channel::UpdateDelayS() {
  delay_s_ = distance_m_ / kSpeedOfLightMPerS;
}

}  // namespace collaborate
}  // namespace osse
