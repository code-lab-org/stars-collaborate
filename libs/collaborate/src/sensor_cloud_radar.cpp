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

#include "collaborate/sensor_cloud_radar.h"

#include <cstdint>
#include <string>

#include "collaborate/earth_data.h"
#include "collaborate/sensor.h"

namespace osse {
namespace collaborate {

SensorCloudRadar::SensorCloudRadar(const std::string& _path,
                                   const uint64_t& _duration_s)
    : Sensor(new EarthData(_path), "TAUTOT", 31, _duration_s, 200) {
}

}  // namespace collaborate
}  // namespace osse
