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

#include "collaborate/battery.h"

namespace osse {
namespace collaborate {

Battery::Battery(const double& _cell_amp_hr,
                 const double& _num_cells,
                 const double& _voltage_v,
                 const double& _charging_efficiency_percent)
    : kCapacityWHr_(_num_cells * _cell_amp_hr * _voltage_v),
      kChargeEfficiencyPercent_(_charging_efficiency_percent),
      energy_w_hr_(kCapacityWHr_) {
}

void Battery::IntroduceEnergy(const double& _energy_w_hr) {
  energy_w_hr_ += _energy_w_hr;
  if (energy_w_hr_ < 0) {
    energy_w_hr_ = 0;
  }
  if (energy_w_hr_ > kCapacityWHr_) {
    energy_w_hr_ = kCapacityWHr_;
  }
}

}  // namespace collaborate
}  // namespace osse
