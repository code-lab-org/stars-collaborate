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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_RAW_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_RAW_H_

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include "collaborate/packet.h"

namespace osse {
namespace collaborate {

/// @class PacketRaw
/// @brief A packet of raw measurement data
class PacketRaw : public Packet {
 public:
  /// @brief Maximum size of a route
  static constexpr int kPacketRawSizeBytes = 108;
  /// @brief Maximum size of a route
  static constexpr int kMaxStringSize = 30;
  /// @brief Number of element in a raw packet
  static constexpr int kNumElements = 15;
  /// @brief Index of elapsed (seconds)
  static constexpr int kElapsedSIndex = 0;
  /// @brief Index of year
  static constexpr int kYearIndex = 8;
  /// @brief Index of month
  static constexpr int kMonthIndex = 12;
  /// @brief Index of day
  static constexpr int kDayIndex = 16;
  /// @brief Index of hour
  static constexpr int kHourIndex = 20;
  /// @brief Index of minute
  static constexpr int kMinuteIndex = 24;
  /// @brief Index of second
  static constexpr int kSecondIndex = 28;
  /// @brief Index of microsecond
  static constexpr int kMicrosecondIndex = 32;
  /// @brief Index of latitude (radians)
  static constexpr int kLatitudeRadIndex = 36;
  /// @brief Index of longitude (radians)
  static constexpr int kLongitudeRadIndex = 44;
  /// @brief Index of altitude (meters)
  static constexpr int kAltitudeMIndex = 52;
  /// @brief Index of measurement
  static constexpr int kMeasurementIndex = 60;
  /// @brief Index of resolution radius (meters)
  static constexpr int kResolutionRadMIndex = 68;
  /// @brief Index of name
  static constexpr int kNameIndex = 76;
  /// @brief Index of informer index
  static constexpr int kInformerIndexIndex = 106;
  /// @brief Constructor from payload
  /// @param[in] _payload Payload
  explicit PacketRaw(std::vector<uint8_t> _payload);
  /// @brief Constructor from data members
  /// @param[in] _elapsed_s Time elapsed in the simulation (seconds)
  /// @param[in] _year Year
  /// @param[in] _month Month
  /// @param[in] _day Day
  /// @param[in] _hour Hour
  /// @param[in] _minute Minute
  /// @param[in] _second Second
  /// @param[in] _microsecond Microsecond
  /// @param[in] _latitude_rad Latitude (radians)
  /// @param[in] _longitude_rad Longitude (radians)
  /// @param[in] _altitude_m Altitude (meters)
  /// @param[in] _measurement Measurement
  /// @param[in] _resolution_radius_m Resolution (meters)
  /// @param[in] _name Variable name of data set
  /// @param[in] _informer_index Informer index index
  PacketRaw(const uint64_t& _elapsed_s,
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
            const uint16_t& _informer_index);
  /// @brief Converts the packet to a string
  /// @returns The packet as a string
  std::string ToString() const;
  /// @brief Get Year
  /// @returns year_ Year
  const uint64_t& elapsed_s() const {return elapsed_s_;}
  /// @brief Get Year
  /// @returns year_ Year
  const int& year() const {return year_;}
  /// @brief Get Month
  /// @returns month_ Month
  const int& month() const {return month_;}
  /// @brief Get Day
  /// @returns day_ Day
  const int& day() const {return day_;}
  /// @brief Get Hour
  /// @returns hour_ Hour
  const int& hour() const {return hour_;}
  /// @brief Get Minute
  /// @returns minute_ Minute
  const int& minute() const {return minute_;}
  /// @brief Get Second
  /// @returns second_ Second
  const int& second() const {return second_;}
  /// @brief Get Microsecond
  /// @returns microsecond_ Microsecond
  const int& microsecond() const {return microsecond_;}
  /// @brief Get Latitude (radians)
  /// @returns latitude_rad_ Latitude (radians)
  const double& latitude_rad() const {return latitude_rad_;}
  /// @brief Get Longitude (radians)
  /// @returns longitude_rad_ Longitude (radians)
  const double& longitude_rad() const {return longitude_rad_;}
  /// @brief Get Altitude (meters)
  /// @returns altitude_m_ Altitude (meters)
  const double& altitude_m() const {return altitude_m_;}
  /// @brief Get Measurement
  /// @returns measurement_ Measurement
  const double& measurement() const {return measurement_;}
  /// @brief Get Resolution radius (meters)
  /// @returns resolution_radius_m_ Resolution radius (meters)
  const double& resolution_radius_m() const {return resolution_radius_m_;}
  /// @brief Get Variable name of the data-set
  /// @returns name_ Variable name of the data-set
  const std::string& name() const {return name_;}
  /// @brief Get informer index
  /// @returns informer_index_index_ Informer index
  const uint16_t& informer_index() const {return informer_index_;}

 private:
  /// @brief Inserts data members into the payload
  /// @param[in] _elapsed_s Time elapsed in the simulation (seconds)
  /// @param[in] _year Year
  /// @param[in] _month Month
  /// @param[in] _day Day
  /// @param[in] _hour Hour
  /// @param[in] _minute Minute
  /// @param[in] _second Second
  /// @param[in] _microsecond Microsecond
  /// @param[in] _latitude_rad Latitude (radians)
  /// @param[in] _longitude_rad Longitude (radians)
  /// @param[in] _altitude_m Altitude (meters)
  /// @param[in] _measurement Measurement
  /// @param[in] _resolution_radius_m Resolution (meters)
  /// @param[in] _name Variable name of data set
  /// @param[in] _informer_index Informer index index
  /// @returns Serialized version of the raw packet
  std::vector<uint8_t> PackAll(const uint64_t& _elapsed_s,
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
                               const uint16_t& _informer_index);
  /// @brief Inserts a string into the payload
  /// @param[in] _string String
  /// @param[in] _payload Payload
  void PackString(const std::string& _string,
                  std::vector<uint8_t>* _payload) const;
  /// @brief Unpacks a string from the payload
  /// @param[in] _payload Payload
  /// @param[in] _index Index
  /// @returns Unpacked string
  std::string UnpackString(const std::vector<uint8_t>& _payload,
                           const uint16_t& _index) const;
  /// @brief Time elapsed since the beginning of the simulation (seconds)
  uint64_t elapsed_s_;
  /// @brief Year
  int year_;
  /// @brief Month
  int month_;
  /// @brief Day
  int day_;
  /// @brief Hour
  int hour_;
  /// @brief Minute
  int minute_;
  /// @brief Second
  int second_;
  /// @brief Microsecond
  int microsecond_;
  /// @brief Latitude (radians)
  double latitude_rad_;
  /// @brief Longitude (radians)
  double longitude_rad_;
  /// @brief Altitude (meters)
  double altitude_m_;
  /// @brief Measurement
  double measurement_;
  /// @brief Resolution radius (meters)
  double resolution_radius_m_;
  /// @brief Variable name of the data-set
  std::string name_;
  /// @brief Informer index
  uint16_t informer_index_;
};

/// @brief Output raw packet to a stream
/// @param[in,out] strm stream to output to
/// @param[in] _raw_packet
inline std::ostream& operator<<(std::ostream& strm,
                                const PacketRaw& _raw_packet) {
  return strm << _raw_packet.ToString();
}

/// @fn std::vector<PacketRaw> ReadRawBuffer(std::vector<uint8_t> _buffer)
/// @brief Obtains a list of raw packets from the data buffer
/// @param[in] _data_buffer Data buffer
/// @returns Raw packets
std::vector<PacketRaw> ReadRawBuffer(const std::vector<uint8_t>& _buffer);

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_RAW_H_
