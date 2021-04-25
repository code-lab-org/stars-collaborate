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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUBSYSTEM_SENSING_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUBSYSTEM_SENSING_H_

#include <cstdint>
#include <vector>

#include "collaborate/antenna.h"
#include "collaborate/data_logger.h"
#include "collaborate/reference_frame.h"
#include "collaborate/sensor.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

/// @class SubsystemSensing
/// @brief An interface for RF sensing
class SubsystemSensing {
 public:
  /// @brief A buffer for logged node data
  typedef struct LogBuffer {
    /// @brief Elapsed time (seconds)
    std::vector<uint64_t> elapsed_s;
    /// @brief Year
    std::vector<int> year;
    /// @brief Month
    std::vector<int> month;
    /// @brief Day
    std::vector<int> day;
    /// @brief Hour
    std::vector<int> hour;
    /// @brief Minute
    std::vector<int> minute;
    /// @brief Second
    std::vector<int> second;
    /// @brief Microsecond
    std::vector<int> microsecond;
    /// @brief Latitude (radians)
    std::vector<double> latitude_rad;
    /// @brief Longitude (radians)
    std::vector<double> longitude_rad;
    /// @brief Altitude (meters)
    std::vector<double> altitude_m;
    /// @brief Meaurement
    std::vector<double> measurement;
    /// @brief Measurement resolution (m)
    std::vector<double> resolution_m;
    /// @brief Node index
    std::vector<uint16_t> index;
  } LogBuffer;
  /// @brief Constructor
  /// @param[in] _antenna The antenna
  /// @param[in] _sensor The sensor
  SubsystemSensing(const Antenna* _antenna, Sensor* _sensor);
  /// @brief Measures for a specified time
  /// @param[in] _informer_index Index of the informer node
  void Measure(const uint16_t& _informer_index);
  /// @brief Obtain a measurement from the earth_data
  /// @param[in] _clock The simulation clock
  /// @param[in] _position_m_rad The position of the node
  /// @param[in] _node_index Unknown
  /// @returns active_
  bool Update(const SimulationClock& _clock, const Vector& _position_m_rad, const int _node_index);
  /// @brief Calculates a new attitude frame for the antenna
  /// @param _orbit_frame Satellite orbit frame
  /// @param _body_frame Satellite body frame
  void OrientAntenna(const ReferenceFrame& _orbit_frame,
                     const ReferenceFrame& _body_frame);
  /// @brief Adds data to the data buffer
  /// @param[in] _payload Payload
  void LoadData(const std::vector<uint8_t>& _payload);
  /// @brief Empties data buffer
  void EraseDataBuffer();
  /// @brief Set data buffer
  /// @param[in] _data_buffer Data buffer
  void set_data_buffer(const std::vector<uint8_t>& _data_buffer);
  /// @brief Get Antenna
  /// @returns kAntenna_ Antenna
  const Antenna* kAntenna() const {return kAntenna_;}
  /// @brief Get Antenna reference frame
  /// @returns antenna_frame_ Antenna reference frame
  ReferenceFrame antenna_frame() const {return antenna_frame_;}
  /// @brief Get Data buffer
  /// @returns data_buffer_ Data buffer
  const std::vector<uint8_t>& data_buffer() const {return data_buffer_;}
  /// @brief Activity status
  /// @returns active_ Activity status
  const bool& active() const {return active_;}
  /// @brief Get time counter (seconds)
  /// @returns elapsed_s_ Time counter (seconds)
  const uint64_t& elapsed_s() const {return elapsed_s_;}
  /// @brief Sets the complete flag
  /// @param[in] _complete
  void set_complete(const bool& _complete) {complete_ = _complete;}
  /// @brief Get The sensor
  /// @returns sensor_ The sensor
  Sensor* sensor() const {return sensor_;}
  /// @brief Get The time limit for activity
  /// @returns expiration_s_ The time limit for activity
  const uint64_t& expiration_s() const {return expiration_s_;}
  /// @brief Get whether or not the measurement is complete
  /// @returns complete_ Whether or not the measurement is complete
  const bool& complete() const {return complete_;}

 private:
  /// @brief Output the logged data to a netcdf file
  /// @param[in] _clock The simulation clock
  /// @param[in] _node_index Unknown
  void Flush(const SimulationClock& _clock, const int _node_index);
  /// @brief The sensor
  Sensor* sensor_;
  /// @brief The time limit for activity
  uint64_t expiration_s_;
  /// @brief Whether or not the measurement is complete
  bool complete_;
  /// @brief Index of the informer node
  uint16_t informer_index_;
  /// @brief Buffer for logged data
  LogBuffer buffer_;
  /// @brief Antenna
  const Antenna* kAntenna_;
  /// @brief Antenna reference frame
  ReferenceFrame antenna_frame_;
  /// @brief Data buffer
  std::vector<uint8_t> data_buffer_;
  /// @brief Activity status
  bool active_;
  /// @brief Get time counter (seconds)
  uint64_t elapsed_s_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUBSYSTEM_SENSING_H_
