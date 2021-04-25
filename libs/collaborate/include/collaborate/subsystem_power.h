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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUBSYSTEM_POWER_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUBSYSTEM_POWER_H_

#include <vector>

#include "collaborate/battery.h"
#include "collaborate/reference_frame.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/solar_panel.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

/// @class SubsystemPower
/// @image html power.png
/// @image html raster/power.png width=500
/// @image latex vector/power.pdf width=\textwidth
class SubsystemPower {
 public:
  /// @brief Number of seconds per hour
  static constexpr int kSecsPerHr = 3600;
  /// @brief Constructor
  /// @param[in] _battery Battery
  /// @param[in] _solar_panels Solar panels
  /// @param[in] _idle_power_w Idle power consumed
  SubsystemPower(const Battery& _battery,
                 const std::vector<SolarPanel> _solar_panels,
                 const double& _idle_power_w);
  /// @brief Updates the system's net energy stored
  /// @param[in] _charge Whether to charge the battery
  /// @param[in] _simulation_clock Simulation simulation_clock
  /// @param[in] _body_frame Host body reference_frame
  /// @param[in] _orbit_frame Host orbit reference_frame
  /// @param[in] _power_drain_w Power drained since last update
  /// @param[in] _position_m_rad Host's position (meters and radians)
  void Update(const bool& _charge,
              const SimulationClock& _simulation_clock,
              const ReferenceFrame& _body_frame,
              const ReferenceFrame& _orbit_frame,
              const double& _power_drain_w,
              const Vector& _position_m_rad);
  /// @brief Get Battery
  /// @returns battery_ Battery
  const Battery& battery() const {return battery_;}
  /// @brief Get Panels
  /// @returns solar_panels_ Panels
  const std::vector<SolarPanel>& solar_panels() const {return solar_panels_;}
  /// @brief Get whether the system is charging
  /// @returns charging_ Whether the system is charging
  const bool& charging() const {return charging_;}

 private:
  /// @brief Whether the system is charging
  bool charging_;
  /// @brief Battery
  Battery battery_;
  /// @brief List of solar panels
  std::vector<SolarPanel> solar_panels_;
  /// @brief Idle consumed power
  double idle_power_w_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUBSYSTEM_POWER_H_
