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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SENSOR_RAIN_RADAR_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SENSOR_RAIN_RADAR_H_

#include <cstdint>
#include <string>

#include "collaborate/sensor.h"

namespace osse {
namespace collaborate {

/// @class SensorRainRadar
/// @brief Measures total precipitation
class SensorRainRadar : public Sensor {
 public:
  /// @brief Constructor
  /// @param[in] _path Parent directory path
  /// @param[in] _duration_s Duration of measurements (seconds)
  SensorRainRadar(const std::string& _path, const uint64_t& _duration_s);
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SENSOR_RAIN_RADAR_H_
