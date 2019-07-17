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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SENSOR_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SENSOR_H_

#include <cstdint>
#include <memory>
#include <string>

#include "collaborate/simulation_clock.h"
#include "collaborate/earth_data.h"

namespace osse {
namespace collaborate {

/// @class Sensor
/// @brief Measures scientific data
class Sensor {
 public:
  /// @brief Constructor from earth_data
  /// @param[in] _earth_data EarthData
  /// @param[in] _variable Variable name
  /// @param[in] _power_consumed_w Power consumed (Watts)
  /// @param[in] _duration_s Duration of measurements (seconds)
  /// @param[in] _target_value Target value for measurements
  Sensor(EarthData* _earth_data,
         const std::string& _variable,
         const double& _power_consumed_w,
         const uint64_t& _duration_s,
         const double& _target_value);
  /// @brief Get The earth_data of scientific data
  /// @returns earth_data_ The earth_data of scientific data
  EarthData* earth_data() const {return earth_data_.get();}
  /// @brief Get variable name
  /// @returns kVariable_ Variable name
  const std::string& kVariable() const {return kVariable_;}
  /// @brief Get power consumed (Watts)
  /// @returns kPowerConsumedW_ Power consumed (Watts)
  const double& kPowerConsumedW() const {return kPowerConsumedW_;}
  /// @brief Get duration of measurements (seconds)
  /// @returns kDurationS_ Duration of measurements (seconds)
  const uint64_t& kDurationS() const {return kDurationS_;}
  /// @brief Get target value for measurements
  /// @returns kTargetValue_ Target value for measurements
  const double& kTargetValue() const {return kTargetValue_;}
  /// @brief Updates the earth_data
  /// @param[in] _clock Simulation clock
  void Update(const SimulationClock& _clock) const;

 private:
  /// @brief Data earth_data
  std::unique_ptr<EarthData> earth_data_;
  /// @brief Variable name
  const std::string kVariable_;
  /// @brief Power consumed during operation (Watts)
  const double kPowerConsumedW_;
  /// @brief Duration of measurements (seconds)
  const uint64_t kDurationS_;
  /// @brief Target value for measurements
  const double kTargetValue_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SENSOR_H_
