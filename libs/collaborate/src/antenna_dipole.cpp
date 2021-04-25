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

#include "collaborate/antenna_dipole.h"

#include <cmath>

#include "collaborate/antenna.h"

namespace osse {
namespace collaborate {

AntennaDipole::AntennaDipole(const double& _max_gain_db,
                             const double& _roll_rad,
                             const double& _pitch_rad,
                             const double& _yaw_rad)
    : Antenna(_max_gain_db, _roll_rad, _pitch_rad, _yaw_rad) {
}

double AntennaDipole::GainDb(const double& _theta_rad,
                             const double& _phi_rad) const {
  // Gain is symmetrical in phi and constant in omega
  (void)(_phi_rad);
  return kMaxGainDb_ * std::pow(std::sin(_theta_rad), 2);
}

}  // namespace collaborate
}  // namespace osse
