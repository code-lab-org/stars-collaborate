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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_ANTENNA_ISOTROPIC_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_ANTENNA_ISOTROPIC_H_

#include "collaborate/antenna.h"

namespace osse {
namespace collaborate {

/// @class AntennaIsotropic
/// @brief An isotropic antenna
/// @image html raster/isotropic.png width=350
/// @image latex vector/isotropic.pdf width=0.5\textwidth
/// @details
/// \f[ g = g_{max}~~~(decibels) \f]
class AntennaIsotropic : public Antenna {
 public:
  /// @brief Constructor
  /// @param[in] _max_gain_db Maximum gain (decibels)
  explicit AntennaIsotropic(const double& _max_gain_db);
  /// @brief Obtain directional gain (decibels)
  /// @param[in] _theta_rad Altitude angle from positive z-axis (radians)
  /// @param[in] _phi_rad Azimuth angle (radians)
  /// @returns Directional gain (decibels)
  /// @details
  /// \f[ g = g_{max}~~~(decibels) \f]
  double GainDb(const double& _theta_rad, const double& _phi_rad) const;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_ANTENNA_ISOTROPIC_H_
