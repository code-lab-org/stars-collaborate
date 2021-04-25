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

#include "collaborate/subsystem_power.h"

#include <limits>
#include <vector>

#include "collaborate/battery.h"
#include "collaborate/reference_frame.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/solar_panel.h"

namespace osse {
namespace collaborate {

SubsystemPower::SubsystemPower(const Battery& _battery,
                               const std::vector<SolarPanel> _solar_panels,
                               const double& _idle_power_w)
    : charging_(false),
      battery_(_battery),
      solar_panels_(_solar_panels),
      idle_power_w_(_idle_power_w) {
}

void SubsystemPower::Update(const bool& _charge,
                            const SimulationClock& _clock,
                            const ReferenceFrame& _body_frame,
                            const ReferenceFrame& _orbit_frame,
                            const double& _power_drain_w,
                            const Vector& _position_m_rad) {
  // Subtract power drained since last update
  battery_.IntroduceEnergy((-1.0
                            * (idle_power_w_ + _power_drain_w)
                            * _clock.last_increment_s()
                            / kSecsPerHr));

  // Add energy received from each solar panel
  double accumulated_energy_w_hr = 0;
  bool charging = false;
  for (auto panel : solar_panels_) {
    panel.Update(_body_frame, _orbit_frame, _position_m_rad);
    accumulated_energy_w_hr += (panel.RxPowerW()
                                * _clock.last_increment_s()
                                / kSecsPerHr);
    if (panel.RxPowerW() > 0) {
      charging = true;
    }
  }
  charging_ = charging;
  if (_charge) {
    battery_.IntroduceEnergy(accumulated_energy_w_hr
                             * battery_.kChargeEfficiencyPercent()
                             / 100);
  }
}

}  // namespace collaborate
}  // namespace osse
