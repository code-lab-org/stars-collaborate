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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_PLATFORM_EARTH_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_PLATFORM_EARTH_H_

#include <cstdint>
#include <string>
#include <vector>

#include "collaborate/geodetic.h"
#include "collaborate/orbital_state.h"
#include "collaborate/platform.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

/// @class PlatformEarth
/// @brief Propogates the position of a stationary object on Earth's surface
class PlatformEarth : public Platform {
 public:
  /// @brief Constructor From LLH
  /// @param[in] _name Name of the platform
  /// @param[in] _latitude_rad Latitude (radians)
  /// @param[in] _longitude_rad Longitude (radians)
  /// @param[in] _altitude_m Altitude (meters)
  PlatformEarth(const std::string& _name,
                const double& _latitude_rad,
                const double& _longitude_rad,
                const double& _altitude_m);
  /// @brief Get the orbital_state of an object using the current clock value
  /// @param[in] _clock Simulation clock
  /// @param[in] _time_s Time offset from the current time (seconds)
  /// @returns orbital_state OrbitalState of an obect at the specified time
  OrbitalState PredictOrbitalState(const SimulationClock& _clock,
                                   const uint64_t& _time_s) const;
  /// @brief Update the orbital_state of an object the current clock time
  /// @param[in] _clock Simulation clock
  /// @param[in] _time_s Time offset from the current time (seconds)
  /// @param[in] _orbital_state OrbitalState
  void PredictOrbitalState(const SimulationClock& _clock,
                           const uint64_t& _time_s,
                           OrbitalState* _orbital_state) const;
  /// @brief Get geodetic position
  /// @returns geodetic_ Geodetic position
  const Geodetic& kGeodeticRadM() const {return kGeodeticRadM_;}

 private:
  /// @brief Geodetic position
  const Geodetic kGeodeticRadM_;
};

/// @fn std::vector<PlatformEarth> PlatformEarthList(std::string _path)
/// @brief Constructs a list of PlatformEarth objects from an input file
/// @param[in] _path File path
/// @returns orbits Vector of PlatformEarths constructed from file
std::vector<PlatformEarth> PlatformEarthList(const std::string& _path);

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_PLATFORM_EARTH_H_
