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

#include "collaborate/solar_panel.h"

#include <cmath>

#include "collaborate/earth.h"
#include "collaborate/reference_frame.h"
#include "collaborate/sun.h"
#include "collaborate/util.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

SolarPanel::SolarPanel(const double& _efficiency_percent,
                       const double& _surface_area_m2,
                       const double& _roll_rad,
                       const double& _pitch_rad,
                       const double& _yaw_rad,
                       Sun* _sun)
    : kEfficiencyPercent_(_efficiency_percent),
      kSurfaceAreaM2_(_surface_area_m2),
      kRollRad_(_roll_rad),
      kPitchRad_(_pitch_rad),
      kYawRad_(_yaw_rad),
      kSun_(_sun),
      effective_area_m2_(0),
      attitude_(ReferenceFrame(kRollRad_, kPitchRad_, kYawRad_)) {
}

double SolarPanel::RxPowerW() const {
  return (kSolarIrradianceWPerM2
          * effective_area_m2_
          * (kEfficiencyPercent_/100));
}

void SolarPanel::Update(const ReferenceFrame& _body_frame,
                        const ReferenceFrame& _orbit_frame,
                        const Vector& _position_m_rad) {
  attitude_.Update(_orbit_frame, _body_frame);
  Vector sun_position_m_rad = kSun_->PositionMRad();
  Vector sun_direction = _position_m_rad - sun_position_m_rad;
  sun_direction.CompleteCoordinates();
  double angle_rad = attitude_.z_axis().AngleBetween(sun_direction);
  bool line_of_sight = earth::Visible(_position_m_rad, sun_position_m_rad);
  if (line_of_sight && (angle_rad < (util::kPiByTwoRad))) {
    effective_area_m2_ = kSurfaceAreaM2_ * std::cos(angle_rad);
  } else {
    effective_area_m2_ = 0.0;
  }
}

}  // namespace collaborate
}  // namespace osse
