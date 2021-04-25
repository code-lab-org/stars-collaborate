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

#include "collaborate/packet_raw.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace osse {
namespace collaborate {

PacketRaw::PacketRaw(std::vector<uint8_t> _payload)
    : Packet(_payload),
      elapsed_s_(UnpackUInt64T(_payload, kElapsedSIndex)),
      year_(UnpackInteger(_payload, kYearIndex)),
      month_(UnpackInteger(_payload, kMonthIndex)),
      day_(UnpackInteger(_payload, kDayIndex)),
      hour_(UnpackInteger(_payload, kHourIndex)),
      minute_(UnpackInteger(_payload, kMinuteIndex)),
      second_(UnpackInteger(_payload, kSecondIndex)),
      microsecond_(UnpackInteger(_payload, kMicrosecondIndex)),
      latitude_rad_(UnpackDouble(_payload, kLatitudeRadIndex)),
      longitude_rad_(UnpackDouble(_payload, kLongitudeRadIndex)),
      altitude_m_(UnpackDouble(_payload, kAltitudeMIndex)),
      measurement_(UnpackDouble(_payload, kMeasurementIndex)),
      resolution_radius_m_(UnpackDouble(_payload, kResolutionRadMIndex)),
      name_(UnpackString(_payload, kNameIndex)),
      informer_index_(UnpackUInt16T(_payload, kInformerIndexIndex)) {
}

PacketRaw::PacketRaw(const uint64_t& _elapsed_s,
                     const int& _year,
                     const int& _month,
                     const int& _day,
                     const int& _hour,
                     const int& _minute,
                     const int& _second,
                     const int& _microsecond,
                     const double& _latitude_rad,
                     const double& _longitude_rad,
                     const double& _altitude_m,
                     const double& _measurement,
                     const double& _resolution_radius_m,
                     const std::string& _name,
                     const uint16_t& _informer_index)
    : Packet(PackAll(_elapsed_s,
                     _year,
                     _month,
                     _day,
                     _hour,
                     _minute,
                     _second,
                     _microsecond,
                     _latitude_rad,
                     _longitude_rad,
                     _altitude_m,
                     _measurement,
                     _resolution_radius_m,
                     _name,
                     _informer_index)),
      elapsed_s_(_elapsed_s),
      year_(_year),
      month_(_month),
      day_(_day),
      hour_(_hour),
      minute_(_minute),
      second_(_second),
      microsecond_(_microsecond),
      latitude_rad_(_latitude_rad),
      longitude_rad_(_longitude_rad),
      altitude_m_(_altitude_m),
      measurement_(_measurement),
      resolution_radius_m_(_resolution_radius_m),
      name_(_name),
      informer_index_(_informer_index) {
}

std::string PacketRaw::ToString() const {
  std::stringstream stream;
  stream << "Elapsed (s): " << elapsed_s_ << std::endl
         << "Year: " << year_ << std::endl
         << "Month: " << month_ << std::endl
         << "Day: " << day_ << std::endl
         << "Hour: " << hour_ << std::endl
         << "Minute: " << minute_ << std::endl
         << "Second: " << second_ << std::endl
         << "Microsecond: " << microsecond_ << std::endl
         << "Latitude (rad): " << latitude_rad_ << std::endl
         << "Longitude (rad): " << longitude_rad_ << std::endl
         << "Altitude (m): " << altitude_m_ << std::endl
         << "Measurement: " << measurement_ << std::endl
         << "Resolution Radius (m): " << resolution_radius_m_ << std::endl
         << "Name: " << name_ << std::endl
         << "Informer Index: " << informer_index_ << std::endl;
  return stream.str();
}

std::vector<uint8_t> PacketRaw::PackAll(const uint64_t& _elapsed_s,
                                        const int& _year,
                                        const int& _month,
                                        const int& _day,
                                        const int& _hour,
                                        const int& _minute,
                                        const int& _second,
                                        const int& _microsecond,
                                        const double& _latitude_rad,
                                        const double& _longitude_rad,
                                        const double& _altitude_m,
                                        const double& _measurement,
                                        const double& _resolution_radius_m,
                                        const std::string& _name,
                                        const uint16_t& _informer_index) {
  std::vector<uint8_t> payload;
  Pack(_elapsed_s, &payload);
  Pack(_year, &payload);
  Pack(_month, &payload);
  Pack(_day, &payload);
  Pack(_hour, &payload);
  Pack(_minute, &payload);
  Pack(_second, &payload);
  Pack(_microsecond, &payload);
  Pack(_latitude_rad, &payload);
  Pack(_longitude_rad, &payload);
  Pack(_altitude_m, &payload);
  Pack(_measurement, &payload);
  Pack(_resolution_radius_m, &payload);
  PackString(_name, &payload);
  Pack(_informer_index, &payload);
  return payload;
}

void PacketRaw::PackString(const std::string& _string,
                           std::vector<uint8_t>* payload) const {
  std::string local = _string;
  for (char& c : local) {
    payload->push_back(*reinterpret_cast<uint8_t *>(&c));
  }
}

std::string PacketRaw::UnpackString(const std::vector<uint8_t>& payload,
                                    const uint16_t& _index) const {
  std::vector<uint8_t> local = payload;
  std::stringstream unpacked;
  for (int c = 0; c < kMaxStringSize; ++c) {
    unpacked << *reinterpret_cast<char*>(&local[_index + c]);
  }
  return unpacked.str();
}

std::vector<PacketRaw> ReadRawBuffer(const std::vector<uint8_t>& _buffer) {
  if (_buffer.size() % PacketRaw::kPacketRawSizeBytes != 0) {
    exit(0);
  }
  uint64_t counter = 0;
  std::vector<PacketRaw> messages;
  while (counter < _buffer.size()) {
    int end_index = counter + PacketRaw::kPacketRawSizeBytes;
    std::vector<uint8_t> payload(&_buffer[counter], &_buffer[end_index]);
    PacketRaw message(payload);
    messages.push_back(message);
    counter += PacketRaw::kPacketRawSizeBytes;
  }
  return messages;
}

}  // namespace collaborate
}  // namespace osse
