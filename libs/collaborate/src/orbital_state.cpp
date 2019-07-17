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

#include "collaborate/orbital_state.h"

#include <vector>

#include "collaborate/reference_frame.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

OrbitalState::OrbitalState(double _x_m,
                           double _y_m,
                           double _z_m,
                           double _latitude_rad,
                           double _longitude_rad,
                           double _altitude_m,
                           double _dx_m_per_s,
                           double _dy_m_per_s,
                           double _dz_m_per_s,
                           double _roll_rad,
                           double _pitch_rad,
                           double _yaw_rad)
    : position_m_rad_(Vector(_x_m, _y_m, _z_m)),
      velocity_m_per_s_(Vector(_dx_m_per_s, _dy_m_per_s, _dz_m_per_s)),
      geodetic_rad_m_(Geodetic(_latitude_rad, _longitude_rad, _altitude_m)),
      orbit_frame_(CalculatePlatformOrbitReferenceFrame()),
      body_frame_(ReferenceFrame(orbit_frame_,
                                 _roll_rad,
                                 _pitch_rad,
                                 _yaw_rad)) {
}

ReferenceFrame OrbitalState::CalculatePlatformOrbitReferenceFrame() const {
  Vector y_axis((-position_m_rad_).Cross(velocity_m_per_s_).Unit());
  Vector z_axis((-position_m_rad_).Unit());
  Vector x_axis(y_axis.Cross(z_axis));
  return ReferenceFrame(x_axis, y_axis, z_axis);
}

void OrbitalState::Update(double _x_m,
                          double _y_m,
                          double _z_m,
                          double _latitude_rad,
                          double _longitude_rad,
                          double _altitude_m,
                          double _dx_m_per_s,
                          double _dy_m_per_s,
                          double _dz_m_per_s) {
  position_m_rad_ = Vector(_x_m, _y_m, _z_m);
  velocity_m_per_s_ = Vector(_dx_m_per_s, _dy_m_per_s, _dz_m_per_s);
  geodetic_rad_m_ = Geodetic(_latitude_rad, _longitude_rad, _altitude_m);
  orbit_frame_ = CalculatePlatformOrbitReferenceFrame();
  body_frame_.Update(orbit_frame_);
}

std::vector<double> OrbitalState::ObtainLog() const {
  std::vector<double> log;
  std::vector<double> temp;
  temp = position_m_rad_.ObtainLog();
  log.insert(log.end(), temp.begin(), temp.end());
  temp = geodetic_rad_m_.ObtainLog();
  log.insert(log.end(), temp.begin(), temp.end());
  temp = velocity_m_per_s_.ObtainLog();
  log.insert(log.end(), temp.begin(), temp.end());
  temp = orbit_frame_.ObtainLog();
  log.insert(log.end(), temp.begin(), temp.end());
  temp = body_frame_.ObtainLog();
  log.insert(log.end(), temp.begin(), temp.end());
  return log;
}

std::vector<double> OrbitalState::ObtainGeodeticLog() const {
  std::vector<double> log;
  std::vector<double> geodetic_log = geodetic_rad_m_.ObtainLog();
  log.insert(std::end(log), std::begin(geodetic_log), std::end(geodetic_log));
  return log;
}

}  // namespace collaborate
}  // namespace osse
