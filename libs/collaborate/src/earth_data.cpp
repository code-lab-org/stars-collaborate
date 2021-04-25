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

#include "collaborate/earth_data.h"

#include <experimental/filesystem>

#include <array>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "netcdf/ncFile.h"
#include "netcdf/ncVar.h"

#include "collaborate/earth.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/util.h"

namespace osse {
namespace collaborate {

EarthData::EarthData(const std::string& _root)
    : data_paths_(FindDataPaths(_root)),
      current_index_(std::numeric_limits<uint64_t>::max()),
      kData_({0}) {
}

void EarthData::Update(const SimulationClock& _simulation_clock,
                       const std::string& _variable) {
  constexpr uint64_t kResolutionS_ = 1800;
  uint64_t index = _simulation_clock.elapsed_s() / kResolutionS_;
  if (index != current_index_) {
    current_index_ = index;
    if (data_paths_.size() > current_index_) {
      Buffer(_variable);
    }
  }
}

void EarthData::Buffer(const std::string& _variable) {
  std::string current_path = data_paths_.at(current_index_);
  netCDF::NcFile file(current_path, netCDF::NcFile::read);
  netCDF::NcVar var = file.getVar(_variable);
  var.getVar({0, 0, 0},
             {1, earth::kNumLats, earth::kNumLons},
             kData_.data());
}

float EarthData::Measure(const double& _latitude_rad,
                         const double& _longitude_rad) const {
  return kData_[util::Index(IndexLatitude(_latitude_rad),
                            IndexLongitude(_longitude_rad),
                            earth::kNumLons)];
}

uint16_t EarthData::IndexLatitude(const double& _latitude_rad) const {
  double latitude_rad = -1 * util::kPiByTwoRad;
  uint16_t index = 0;
  bool flag = false;
  while (index < earth::kNumLats && !flag) {
    if (latitude_rad > _latitude_rad) {
      flag = true;
    }
    latitude_rad += earth::kLatIncrementRad;
    ++index;
  }
  return index;
}

uint16_t EarthData::IndexLongitude(const double& _longitude_rad) const {
  double longitude_rad = -1 * util::kPiRad;
  uint16_t index = 0;
  bool flag = false;
  while (index < earth::kNumLons && !flag) {
    if (longitude_rad > _longitude_rad) {
      flag = true;
    }
    longitude_rad += earth::kLonIncrementRad;
    ++index;
  }
  return index;
}

std::vector<std::string> EarthData::FindDataPaths(
    const std::string& _root) const {
  std::vector<std::string> data_paths;
  std::experimental::filesystem::path root(_root);
  for (auto path : std::experimental::filesystem::directory_iterator(root)) {
    if (std::experimental::filesystem::is_regular_file(path)) {
      if (path.path().extension().string() == ".nc4") {
        data_paths.push_back(path.path().string());
      }
    }
  }
  std::sort(data_paths.begin(), data_paths.end());
  return data_paths;
}

}  // namespace collaborate
}  // namespace osse
