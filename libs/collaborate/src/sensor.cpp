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

#include "collaborate/sensor.h"

#include <cstdint>
#include <string>

#include "collaborate/earth_data.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

Sensor::Sensor(EarthData* _earth_data,
               const std::string& _variable,
               const double& _power_consumed_w,
               const uint64_t& _duration_s,
               const double& _target_value)
    : earth_data_(_earth_data),
      kVariable_(_variable),
      kPowerConsumedW_(_power_consumed_w),
      kDurationS_(_duration_s),
      kTargetValue_(_target_value) {
}

void Sensor::Update(const SimulationClock& _clock) const {
  earth_data_->Update(_clock, kVariable_);
}

}  // namespace collaborate
}  // namespace osse
