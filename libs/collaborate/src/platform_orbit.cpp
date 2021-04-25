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

#include "collaborate/platform_orbit.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "sgp4/coord_geodetic.h"
#include "sgp4/date_time.h"
#include "sgp4/sgp4.h"
#include "sgp4/tle.h"

#include "collaborate/orbital_state.h"
#include "collaborate/platform.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/util.h"

namespace osse {
namespace collaborate {

PlatformOrbit::PlatformOrbit(const std::array<std::string,
                             PlatformOrbit::kNumElements>& _tle)
    : Platform(_tle[0]),
      kTle_(_tle),
      kModel_(sgp4::SGP4(sgp4::Tle(kTle_[1], kTle_[2]))) {
}

OrbitalState PlatformOrbit::PredictOrbitalState(
    const SimulationClock& _simulation_clock,
    const uint64_t& _time_s) const {
  sgp4::DateTime future = _simulation_clock.date_time().AddSeconds(_time_s);
  sgp4::Eci eci = kModel_.FindPosition(future);
  sgp4::CoordGeodetic geo = eci.ToGeodetic();
  return OrbitalState(eci.Position().x * 1000.0,
                      eci.Position().y * 1000.0,
                      eci.Position().z * 1000.0,
                      geo.latitude,
                      geo.longitude,
                      geo.altitude * 1000.0,
                      eci.Velocity().x * 1000.0,
                      eci.Velocity().y * 1000.0,
                      eci.Velocity().z * 1000.0,
                      0,
                      0,
                      0);
}

void PlatformOrbit::PredictOrbitalState(
    const SimulationClock& _simulation_clock,
    const uint64_t& _time_s,
    OrbitalState* _orbital_state) const {
  sgp4::DateTime future = _simulation_clock.date_time().AddSeconds(_time_s);
  sgp4::Eci eci = kModel_.FindPosition(future);
  sgp4::CoordGeodetic geo = eci.ToGeodetic();
  _orbital_state->Update(eci.Position().x * 1000.0,
                         eci.Position().y * 1000.0,
                         eci.Position().z * 1000.0,
                         geo.latitude,
                         geo.longitude,
                         geo.altitude * 1000.0,
                         eci.Velocity().x * 1000.0,
                         eci.Velocity().y * 1000.0,
                         eci.Velocity().z * 1000.0);
}

std::vector<PlatformOrbit> PlatformOrbit::Duplicate(
    const uint16_t& _orbit_planes,
    const uint16_t& _groups_per_plane,
    const uint16_t& _sats_in_train,
    const uint16_t& _sats_in_tandem,
    const uint16_t& _train_angle,
    const uint16_t& _tandem_angle) const {
  std::vector<PlatformOrbit> pattern_;
  std::string replacement = kTle_[2];
  uint16_t total_satellites = (_orbit_planes
                               * _groups_per_plane
                               * _sats_in_train
                               * _sats_in_tandem);
  double mean_anomaly = 0.0;
  double right_ascension = 0.0;
  double epoch_right_ascension = atof(kTle_[2].substr(17, 8).c_str());
  double epoch_mean_anomaly = atof(kTle_[2].substr(43, 8).c_str());
  std::string ra;
  std::string ma;
  for (uint16_t p = 0; p < _orbit_planes; ++p) {
    for (uint16_t g = 0; g < _groups_per_plane; ++g) {
      for (uint16_t m = 0; m < _sats_in_train; ++m) {
        for (uint16_t r = 0; r < _sats_in_tandem; ++r) {
          right_ascension = epoch_right_ascension;
          right_ascension += r * _tandem_angle;
          right_ascension += 360.0 * p / _orbit_planes;
          right_ascension = fmod(right_ascension, 360.0);
          mean_anomaly = epoch_mean_anomaly;
          mean_anomaly += m * _train_angle;
          mean_anomaly += 360.0 * g / _groups_per_plane;
          mean_anomaly += 360.0 * p / total_satellites;
          mean_anomaly = fmod(mean_anomaly, 360.0);
          ra = util::StringFromDouble(right_ascension, 3, 4);
          ma = util::StringFromDouble(mean_anomaly, 3, 4);
          replacement.replace(17, 8, ra);
          replacement.replace(43, 8, ma);
          const TwoLineElementSet tle = {kTle_[0], kTle_[1], replacement};
          pattern_.push_back(PlatformOrbit(tle));
        }
      }
    }
  }
  return pattern_;
}

std::vector<PlatformOrbit> PlatformOrbitList(const std::string& _path) {
  std::vector<PlatformOrbit> platforms;
  std::vector<std::string> lines;
  std::string line;
  std::ifstream ifstream(_path, std::ios::in);
  while (std::getline(ifstream, line)) {
    if (!line.empty()) {
      lines.push_back(line);
    }
  }
  unsigned int num_lines = lines.size();
  bool valid_format = ((num_lines % PlatformOrbit::kNumElements) == 0);
  if (valid_format) {
    for (unsigned int i = 0; i < num_lines; i += PlatformOrbit::kNumElements) {
      std::array<std::string, PlatformOrbit::kNumElements> tle = {lines[i],
                                                                  lines[i+1],
                                                                  lines[i+2]};
      PlatformOrbit platform(tle);
      platforms.push_back(platform);
    }
  }
  return platforms;
}

}  // namespace collaborate
}  // namespace osse
