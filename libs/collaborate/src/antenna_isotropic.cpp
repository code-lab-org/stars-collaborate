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

#include "collaborate/antenna_isotropic.h"

#include "collaborate/antenna.h"

namespace osse {
namespace collaborate {

AntennaIsotropic::AntennaIsotropic(const double& _max_gain_db)
    : Antenna(_max_gain_db, 0.0, 0.0, 0.0) {
}

double AntennaIsotropic::GainDb(const double& _theta_rad,
                                const double& _phi_rad) const {
  // Gain is constant in phi, theta, and omega
  (void)(_theta_rad);
  (void)(_phi_rad);
  return kMaxGainDb_;
}

}  // namespace collaborate
}  // namespace osse
