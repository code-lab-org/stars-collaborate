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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUN_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUN_H_

#include "collaborate/simulation_clock.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

/// @class Sun
/// @brief The star at the center of the solar system
/// @image html raster/sun.png width=500
/// @image latex vector/sun.pdf width=\textwidth
class Sun {
 public:
  /// @brief Constructor
  /// @param[in] _clock Simulation clock
  explicit Sun(SimulationClock* _clock);
  /// @brief Update the position
  /// @param[in] _offset_s Offset in time (seconds)
  void Update(const uint64_t& _offset_s);
  /// @brief Obtain position (meters and radians)
  /// @returns Position (meters and radians)
  Vector PositionMRad() const {return position_m_rad_;}

 private:
  /// @brief Simulation clock
  SimulationClock* clock_;
  /// @brief Position (meters and radians)
  Vector position_m_rad_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUN_H_
