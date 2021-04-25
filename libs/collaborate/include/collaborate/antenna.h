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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_ANTENNA_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_ANTENNA_H_

#include <string>

namespace osse {
namespace collaborate {

/// @class Antenna
/// @brief Abstract antenna
/// @image html raster/antennas.png width=500
/// @image latex vector/antennas.pdf width=\textwidth
class Antenna {
 public:
  /// @brief Constructor
  /// @param[in] _max_gain_db Maximum gain (decibels)
  /// @param[in] _roll_rad Roll angle to host body reference frame (radians)
  /// @param[in] _pitch_rad Pitch angle to host body reference frame (radians)
  /// @param[in] _yaw_rad Yaw angle to host body reference frame (radians)
  Antenna(const double& _max_gain_db,
          const double& _roll_rad,
          const double& _pitch_rad,
          const double& _yaw_rad);
  /// @brief Obtain directional gain (decibels)
  /// @param[in] _theta_rad Altitude angle from positive z-axis (radians)
  /// @param[in] _phi_rad Azimuth angle (radians)
  /// @returns Directional gain (decibels)
  virtual double GainDb(const double& _theta_rad,
                        const double& _phi_rad) const = 0;
  /// @brief Logs the antenna pattern to a file
  /// @param[in] _path File path
  void Log(const std::string& _path) const;
  /// @brief Get maximum gain (decibels)
  /// @returns max_gain_db_ Maximum gain (decibels)
  const double& kMaxGainDb() const {return kMaxGainDb_;}
  /// @brief Get roll angle to host body reference frame (radians)
  /// @returns roll_rad_ Roll angle to host body reference frame (radians)
  const double& kRollRad() const {return kRollRad_;}
  /// @brief Get pitch angle to host body reference frame (radians)
  /// @returns pitch_rad_ Pitch angle to host body reference frame (radians)
  const double& kPitchRad() const {return kPitchRad_;}
  /// @brief Get yaw angle to host body reference frame (radians)
  /// @returns yaw_rad_ Yaw angle to host body reference frame (radians)
  const double& kYawRad() const {return kYawRad_;}

 protected:
  /// Maximum gain (decibels)
  const double kMaxGainDb_;
  /// Roll angle to host body reference frame (radians)
  const double kRollRad_;
  /// Pitch angle to host body reference frame (radians)
  const double kPitchRad_;
  /// Yaw angle to host body reference frame (radians)
  const double kYawRad_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_ANTENNA_H_
