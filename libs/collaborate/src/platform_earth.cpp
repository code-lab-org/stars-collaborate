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

#include "collaborate/platform_earth.h"

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "sgp4/coord_geodetic.h"
#include "sgp4/eci.h"

#include "collaborate/geodetic.h"
#include "collaborate/orbital_state.h"
#include "collaborate/platform.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

PlatformEarth::PlatformEarth(const std::string& _name,
                             const double& _latitude_rad,
                             const double& _longitude_rad,
                             const double& _altitude_m)
    : Platform(_name),
      kGeodeticRadM_(Geodetic(_latitude_rad, _longitude_rad, _altitude_m)) {
}

OrbitalState PlatformEarth::PredictOrbitalState(
    const SimulationClock& _simulation_clock,
    const uint64_t& _time_s) const {
  sgp4::CoordGeodetic geo(kGeodeticRadM_.latitude_rad(),
                          kGeodeticRadM_.longitude_rad(),
                          kGeodeticRadM_.altitude_m() / 1000,
                          true);
  sgp4::Eci eci(_simulation_clock.date_time().AddSeconds(_time_s), geo);
  Vector local_position(eci.Position().x * 1000.0,
                        eci.Position().y * 1000.0,
                        eci.Position().z * 1000.0);
  double local_longitude = geo.longitude;
  return OrbitalState(local_position.x_m(),
                      local_position.y_m(),
                      local_position.z_m(),
                      geo.latitude,
                      local_longitude,
                      geo.altitude * 1000.0,
                      eci.Velocity().x * 1000.0,
                      eci.Velocity().y * 1000.0,
                      eci.Velocity().z * 1000.0,
                      0,
                      0,
                      0);
}

void PlatformEarth::PredictOrbitalState(
    const SimulationClock& _simulation_clock,
    const uint64_t& _time_s,
    OrbitalState* _orbital_state) const {
  sgp4::CoordGeodetic geo(kGeodeticRadM_.latitude_rad(),
                          kGeodeticRadM_.longitude_rad(),
                          kGeodeticRadM_.altitude_m() / 1000,
                          true);
  sgp4::Eci eci(_simulation_clock.date_time().AddSeconds(_time_s), geo);
  Vector local_position(eci.Position().x * 1000.0,
                        eci.Position().y * 1000.0,
                        eci.Position().z * 1000.0);
  double local_longitude = geo.longitude;
  _orbital_state->Update(local_position.x_m(),
                         local_position.y_m(),
                         local_position.z_m(),
                         geo.latitude,
                         local_longitude,
                         geo.altitude * 1000.0,
                         eci.Velocity().x * 1000.0,
                         eci.Velocity().y * 1000.0,
                         eci.Velocity().z * 1000.0);
}

std::vector<PlatformEarth> PlatformEarthList(const std::string& _path) {
  std::vector<PlatformEarth> platforms;
  std::vector<std::string> lines;
  std::string line;
  std::ifstream ifstream(_path, std::ios::in);
  while (std::getline(ifstream, line)) {
    if (!line.empty()) {
      lines.push_back(line);
    }
  }
  uint16_t num_lines = lines.size();
  bool valid_format = ((num_lines % 4) == 0);
  if (valid_format) {
    for (uint16_t i = 0; i < num_lines; i += 4) {
      PlatformEarth platform(lines[i],
                             atof(lines[i+1].c_str()),
                             atof(lines[i+2].c_str()),
                             atof(lines[i+3].c_str()));
      platforms.push_back(platform);
    }
  }
  return platforms;
}

}  // namespace collaborate
}  // namespace osse
