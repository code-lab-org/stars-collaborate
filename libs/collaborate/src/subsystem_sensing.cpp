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

#include "collaborate/subsystem_sensing.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <sstream>

#include "collaborate/antenna.h"
#include "collaborate/data_logger.h"
#include "collaborate/geodetic.h"
#include "collaborate/packet_raw.h"
#include "collaborate/reference_frame.h"
#include "collaborate/sensor.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

SubsystemSensing::SubsystemSensing(const Antenna* _antenna,
                                   Sensor* _sensor)
    : sensor_(_sensor),
      expiration_s_(std::numeric_limits<uint64_t>::max()),
      complete_(false),
      informer_index_(std::numeric_limits<uint16_t>::max()),
      buffer_({}),
      kAntenna_(_antenna),
      antenna_frame_(ReferenceFrame(kAntenna_->kRollRad(),
                                    kAntenna_->kPitchRad(),
                                    kAntenna_->kYawRad())),
      data_buffer_(std::vector<uint8_t>()),
      active_(false),
      elapsed_s_(0) {
}

void SubsystemSensing::Measure(const uint16_t& _informer_index) {
  active_ = true;
  informer_index_ = _informer_index;
  elapsed_s_ = 0;
  expiration_s_ = sensor_->kDurationS();
}

bool SubsystemSensing::Update(const SimulationClock& _clock,
                              const Vector& _position_m_rad, const int _node_index) {
  if (active_) {
    sensor_->Update(_clock);
    // From intersection with Earth's surface
    Geodetic place_rad_m(_position_m_rad,
                         antenna_frame_.z_axis(),
                         _clock,
                         0);
    double measurement;
    measurement = sensor_->earth_data()->Measure(place_rad_m.latitude_rad(),
                                                 place_rad_m.longitude_rad());
    std::stringstream stream;
    stream << std::setw(30) << std::setfill(' ') << sensor_->kVariable();
    PacketRaw packet(_clock.elapsed_s(),
                     _clock.date_time().Year(),
                     _clock.date_time().Month(),
                     _clock.date_time().Day(),
                     _clock.date_time().Hour(),
                     _clock.date_time().Minute(),
                     _clock.date_time().Second(),
                     _clock.date_time().Microsecond(),
                     place_rad_m.latitude_rad(),
                     place_rad_m.longitude_rad(),
                     place_rad_m.altitude_m(),
                     measurement,
                     0,
                     stream.str(),
                     _node_index);
    LoadData(packet.payload());
    buffer_.elapsed_s.push_back(_clock.elapsed_s());
    buffer_.year.push_back(_clock.date_time().Year());
    buffer_.month.push_back(_clock.date_time().Month());
    buffer_.day.push_back(_clock.date_time().Day());
    buffer_.hour.push_back(_clock.date_time().Hour());
    buffer_.minute.push_back(_clock.date_time().Minute());
    buffer_.second.push_back(_clock.date_time().Second());
    buffer_.microsecond.push_back(_clock.date_time().Microsecond());
    buffer_.latitude_rad.push_back(place_rad_m.latitude_rad());
    buffer_.longitude_rad.push_back(place_rad_m.longitude_rad());
    buffer_.altitude_m.push_back(place_rad_m.altitude_m());
    buffer_.measurement.push_back(measurement);
    buffer_.resolution_m.push_back(0);
    buffer_.index.push_back(_node_index);
    if (elapsed_s_ < expiration_s_) {
      elapsed_s_ += _clock.last_increment_s();
    } else {
      Flush(_clock, _node_index);
      active_ = false;
      complete_ = true;
      expiration_s_ = std::numeric_limits<uint64_t>::max();
      informer_index_ = std::numeric_limits<uint16_t>::max();
    }
  }
  return active_;
}

void SubsystemSensing::Flush(const SimulationClock& _clock, const int _node_index) {
  std::stringstream stream;
  std::string str = sensor_->kVariable();
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
  stream << "output/"
         << std::fixed
         << std::setw(10)
         << std::setfill('0')
         << _clock.elapsed_s()
         << "s_N"
         << std::fixed
         << std::setw(5)
         << std::setfill('0')
         << _node_index
         << "_"
         << str
         << "_measure.nc4";
  DataLogger log(stream.str());
  uint64_t length = buffer_.elapsed_s.size();
  log.Measurement(length);
  log.LogSeries("time", buffer_.elapsed_s.data(), length);
  log.LogSeries("year", buffer_.year.data(), length);
  log.LogSeries("month", buffer_.month.data(), length);
  log.LogSeries("day", buffer_.day.data(), length);
  log.LogSeries("hour", buffer_.hour.data(), length);
  log.LogSeries("minute", buffer_.minute.data(), length);
  log.LogSeries("second", buffer_.second.data(), length);
  log.LogSeries("microsecond", buffer_.microsecond.data(), length);
  log.LogSeries("latitude", buffer_.latitude_rad.data(), length);
  log.LogSeries("longitude", buffer_.longitude_rad.data(), length);
  log.LogSeries("altitude", buffer_.altitude_m.data(), length);
  log.LogSeries("measurement", buffer_.measurement.data(), length);
  log.LogSeries("resolution", buffer_.resolution_m.data(), length);
  log.LogSeries("index", buffer_.index.data(), length);
  buffer_.elapsed_s.clear();
  buffer_.year.clear();
  buffer_.month.clear();
  buffer_.day.clear();
  buffer_.hour.clear();
  buffer_.minute.clear();
  buffer_.second.clear();
  buffer_.microsecond.clear();
  buffer_.latitude_rad.clear();
  buffer_.longitude_rad.clear();
  buffer_.altitude_m.clear();
  buffer_.measurement.clear();
  buffer_.resolution_m.clear();
  buffer_.index.clear();
}

void SubsystemSensing::OrientAntenna(const ReferenceFrame& _orbit_frame,
                                     const ReferenceFrame& _body_frame) {
  antenna_frame_.Update(_orbit_frame, _body_frame);
}

void SubsystemSensing::LoadData(const std::vector<uint8_t>& _payload) {
  for (auto byte : _payload) {
    data_buffer_.push_back(byte);
  }
}

void SubsystemSensing::EraseDataBuffer() {
  std::vector<uint8_t> empty;
  data_buffer_ = empty;
}

void SubsystemSensing::set_data_buffer(
    const std::vector<uint8_t>& _data_buffer) {
  data_buffer_ = _data_buffer;
}

}  // namespace collaborate
}  // namespace osse
