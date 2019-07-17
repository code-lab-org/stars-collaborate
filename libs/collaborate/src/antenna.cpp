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

#include "collaborate/antenna.h"

#include <string>

#include "collaborate/data_logger.h"
#include "collaborate/util.h"

namespace osse {
namespace collaborate {

Antenna::Antenna(const double& _max_gain_db,
                 const double& _roll_rad,
                 const double& _pitch_rad,
                 const double& _yaw_rad)
    : kMaxGainDb_(_max_gain_db),
      kRollRad_(_roll_rad),
      kPitchRad_(_pitch_rad),
      kYawRad_(_yaw_rad) {
}

void Antenna::Log(const std::string& _path) const {
  constexpr uint64_t kThetaTicks = 100;
  constexpr uint64_t kPhiTicks = 100;
  std::array<double, kThetaTicks * kPhiTicks> gain;
  for (uint16_t t = 0; t < kThetaTicks; ++t) {
    for (uint16_t p = 0; p < kPhiTicks; ++p) {
      // Convert Increments to Real Values
      double theta_rad = util::kPiRad * t / (kThetaTicks-1);
      double phi_rad = util::kTwoPiRad * p / (kPhiTicks-1);
      uint64_t idx = util::Index(t, p, kPhiTicks);
      gain.at(idx) = GainDb(theta_rad, phi_rad);
    }
  }
  DataLogger data_logger(_path);
  data_logger.LogAntenna(kThetaTicks, kPhiTicks, gain.data());
}

}  // namespace collaborate
}  // namespace osse
