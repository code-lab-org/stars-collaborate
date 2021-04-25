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

#include "collaborate/sun.h"

#include "sgp4/date_time.h"
#include "sgp4/eci.h"
#include "sgp4/solar_position.h"

#include "collaborate/simulation_clock.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

Sun::Sun(SimulationClock* _clock) : clock_(_clock) {
}

void Sun::Update(const uint64_t& _offset_s) {
  sgp4::SolarPosition solar_position;
  sgp4::DateTime date_time = clock_->date_time().AddSeconds(_offset_s);
  sgp4::Eci position_km = solar_position.FindPosition(date_time);
  constexpr double kMetersPerKilometer = 1000.0;
  position_m_rad_ = Vector(position_km.Position().x * kMetersPerKilometer,
                           position_km.Position().y * kMetersPerKilometer,
                           position_km.Position().z * kMetersPerKilometer);
}

}  // namespace collaborate
}  // namespace osse
