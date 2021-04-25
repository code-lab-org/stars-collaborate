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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_ANTENNA_HELICAL_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_ANTENNA_HELICAL_H_

#include "collaborate/antenna.h"

namespace osse {
namespace collaborate {

/// @class AntennaHelical
/// @brief An approximation of a helical antenna (more directional than patch)
/// @image html raster/helical.png width=350
/// @image latex vector/helical.pdf width=0.5\textwidth
/// @details
/// \f[ g = g_{max}\cos^{50}{\theta}~~~(decibels) \f]
class AntennaHelical : public Antenna {
 public:
  /// @brief Constructor
  /// @param[in] _max_gain_db Maximum gain (decibels)
  /// @param[in] _roll_rad Roll angle to host body reference frame (radians)
  /// @param[in] _pitch_rad Pitch angle to host body reference frame (radians)
  /// @param[in] _yaw_rad Yaw angle to host body reference frame (radians)
  AntennaHelical(const double& _max_gain_db,
                 const double& _roll_rad,
                 const double& _pitch_rad,
                 const double& _yaw_rad);
  /// @brief Obtain directional gain (decibels)
  /// @param[in] _theta_rad Altitude angle from positive z-axis (radians)
  /// @param[in] _phi_rad Azimuth angle (radians)
  /// @returns Directional gain (decibels)
  /// @details
  /// \f[ g = g_{max}\cos^{50}{\theta}~~~(decibels) \f]
  double GainDb(const double& _theta_rad, const double& _phi_rad) const;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_ANTENNA_HELICAL_H_
