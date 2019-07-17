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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_PLATFORM_ORBIT_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_PLATFORM_ORBIT_H_

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "sgp4/sgp4.h"

#include "collaborate/orbital_state.h"
#include "collaborate/platform.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

/// @class PlatformOrbit
/// @brief Propogates the position of a satellite
/// @image html raster/constellations.png width=500
/// @image latex vector/constellations.pdf width=\textwidth
class PlatformOrbit : public Platform {
 public:
  /// @brief Number of strings in a two-line element set
  static constexpr int kNumElements = 3;
  /// @brief A two-line element set
  typedef std::array<std::string, kNumElements> TwoLineElementSet;
  /// @brief Constructor from TLE
  /// @param[in] _tle Two-line element set
  explicit PlatformOrbit(const TwoLineElementSet& _tle);
  /// @brief Get the orbital_state of an object the current clock time
  /// @param[in] _simulation_clock Simulation clock
  /// @param[in] _time_s Time offset from the current time (seconds)
  /// @returns OrbitalState of an object the current clock time
  OrbitalState PredictOrbitalState(const SimulationClock& _simulation_clock,
                                   const uint64_t& _time_s) const;
  /// @brief Update the orbital_state of an object the current clock time
  /// @param[in] _simulation_clock Simulation clock
  /// @param[in] _time_s Time offset from the current time (seconds)
  /// @param[in] _orbital_state OrbitalState
  void PredictOrbitalState(const SimulationClock& _simulation_clock,
                           const uint64_t& _time_s,
                           OrbitalState* _orbital_state) const;
  /// @brief Makes a pattern of orbits based on this
  /// @param[in] _orbit_planes Number of orbit planes
  /// @param[in] _groups_per_plane Number of groups per plane
  /// @param[in] _sats_in_train Number of satellites in a train
  /// @param[in] _sats_in_tandem Number of satellites in tandem
  /// @param[in] _train_angle Angle between train satellites
  /// @param[in] _tandem_angle Angle between tandem satellites
  /// @returns pattern_ A list of orbits
  std::vector<PlatformOrbit> Duplicate(const uint16_t& _orbit_planes,
                                       const uint16_t& _groups_per_plane,
                                       const uint16_t& _sats_in_train,
                                       const uint16_t& _sats_in_tandem,
                                       const uint16_t& _train_angle,
                                       const uint16_t& _tandem_angle) const;
  /// @brief Get Array of TLE strings
  /// @returns kTle_ Array of TLE strings
  const TwoLineElementSet& kTle() const {return kTle_;}

 private:
  /// @brief Array of TLE strings
  const TwoLineElementSet kTle_;
  /// @brief SGP4 orbital model
  const sgp4::SGP4 kModel_;
};

/// @fn std::vector<PlatformOrbit> PlatformOrbitList(std::string _path)
/// @brief Constructs a list of PlatformOrbit objects from an input file
/// @param[in] _path File path
/// @returns orbits Vector of PlatformOrbits constructed from file
std::vector<PlatformOrbit> PlatformOrbitList(const std::string& _path);

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_PLATFORM_ORBIT_H_
