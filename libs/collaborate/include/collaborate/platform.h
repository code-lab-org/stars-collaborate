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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_PLATFORM_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_PLATFORM_H_

#include <cstdint>
#include <string>

#include "collaborate/orbital_state.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

/// @class Platform
/// @brief Propogates a node's position
class Platform {
 public:
  /// @brief Constructor
  /// @param[in] _name The name of the platform
  explicit Platform(const std::string& _name);
  /// @brief Get the orbital_state of an object the current clock time
  /// @param[in] _clock Simulation clock
  /// @param[in] _time_s Time offset from the current time (seconds)
  /// @returns OrbitalState of an object at the current clock time
  virtual OrbitalState PredictOrbitalState(const SimulationClock& _clock,
                                           const uint64_t& _time_s) const = 0;
  /// @brief Update the orbital_state of an object the current clock time
  /// @param[in] _clock Simulation clock
  /// @param[in] _time_s Time offset from the current time (seconds)
  /// @param[in] _orbital_state OrbitalState of an object
  virtual void PredictOrbitalState(const SimulationClock& _clock,
                                   const uint64_t& _time_s,
                                   OrbitalState* _orbital_state) const = 0;
  /// @brief Gets kName
  /// @returns kName_ Name of the platform
  std::string kName() const {return kName_;}

 private:
  /// @brief Name of the platform
  const std::string kName_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_PLATFORM_H_
