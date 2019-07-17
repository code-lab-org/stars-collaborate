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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_EARTH_DATA_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_EARTH_DATA_H_

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "collaborate/earth.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

/// @class EarthData
/// @brief A map of scientific measurement data
/// @image html raster/combined.png width=500
/// @image latex vector/combined.pdf width=\textwidth
class EarthData {
 public:
  /// @brief Constructor
  /// @param[in] _root Root directory path
  explicit EarthData(const std::string& _root);
  /// @brief Updates the truth data with a new frame of measurement data
  /// @param[in] _clock Simulation clock
  /// @param[in] _variable Variable in nercdf file
  void Update(const SimulationClock& _clock, const std::string& _variable);
  /// @brief Obtains a data sample at the nearest location on the discrete map
  /// @param[in] _latitude_rad Latitude (radians)
  /// @param[in] _longitude_rad Longitude (radians)
  /// @returns The data sample
  float Measure(const double& _latitude_rad,
                const double& _longitude_rad) const;

 private:
  /// @brief Loads netcdf data into the current data frame
  /// @param[in] _variable Variable in nercdf file
  void Buffer(const std::string& _variable);
  /// @brief Obtains the index of the nearest latitude value
  /// @param[in] _latitude_rad Latitude (radians)
  /// @returns Index of the nearest latitude value
  uint16_t IndexLatitude(const double& _latitude_rad) const;
  /// @brief Obtains the index of the nearest longitude value
  /// @param[in] _longitude_rad Longitude (radians)
  /// @returns Index of the nearest longitude value
  uint16_t IndexLongitude(const double& _longitude_rad) const;
  /// @brief Finds all regular paths in a directory
  /// @param[in] _root Root directory path name
  /// @returns List of all regular paths in a directory
  std::vector<std::string> FindDataPaths(const std::string& _root) const;
  /// @brief The list of netcdf files
  const std::vector<std::string> data_paths_;
  /// @brief Current data-set's index
  uint64_t current_index_;
  /// @brief Current data set
  std::array<float, earth::kNumPositions> kData_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_EARTH_DATA_H_
