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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_BATTERY_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_BATTERY_H_

#include <cmath>
#include <string>

namespace osse {
namespace collaborate {

/// @class Battery
/// @brief A battery
/// @image html raster/power.png width=500
/// @image latex vector/power.pdf width=\textwidth
class Battery {
 public:
  /// @brief Constructor
  /// @param[in] _cell_amp_hr Cell capacity (Amp hours)
  /// @param[in] _num_cells Number of cells in battery
  /// @param[in] _voltage_v Full charge voltage (Volts)
  /// @param[in] _charging_efficiency_percent Charge efficiency (percent)
  /// @details
  /// \f[ Q_{capacity} = n_{cells} * Q_{cell} * V ~~~ (Watt \cdot hour) \f]
  Battery(const double& _cell_amp_hr,
          const double& _num_cells,
          const double& _voltage_v,
          const double& _charging_efficiency_percent);
  /// @brief Add or subtract from total energy
  /// @param[in] _energy_w_hr Energy (Watt hours)
  void IntroduceEnergy(const double& _energy_w_hr);
  /// @brief Get charge efficiency (percent)
  /// @returns kChargeEfficiencyPercent_ Charge efficiency (percent)
  const double& kChargeEfficiencyPercent() const {
    return kChargeEfficiencyPercent_;
  }
  /// @brief Get Energy stored in the battery (Watt hours)
  /// @returns energy_watt_hr_ Energy stored in the battery (Watt hours)
  const double& energy_w_hr() const {return energy_w_hr_;}

 private:
  /// @brief Capacity of battery (Watt hours)
  const double kCapacityWHr_;
  /// @brief Charge efficiency (percent)
  const double kChargeEfficiencyPercent_;
  /// @brief Energy stored in battery (Watt hours)
  double energy_w_hr_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_BATTERY_H_
