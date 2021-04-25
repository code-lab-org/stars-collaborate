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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_DATA_PROCESSOR_SOURCE_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_DATA_PROCESSOR_SOURCE_H_

#include <cstdint>
#include <utility>
#include <vector>

#include "collaborate/data_processor.h"
#include "collaborate/geodetic.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

/// @class DataProcessorSource
/// @brief A source/informer satellite's internal data processor
class DataProcessorSource : public DataProcessor {
 public:
  /// @brief Constructor
  DataProcessorSource();
  /// @brief Constructor
  DataProcessorSource(const bool& _flag);
  /// @brief Processes a sensor measurement
  /// @param[in] _raw_packets List of raw measurement packets
  /// @param[in] _source_index Index of source node
  /// @param[in] _clock Simulation clock
  /// @param[in] _min_list List of minimal suggestions
  /// @param[in] _max_list List of maximum suggestions
  /// @param[in] _feedback List of feedback target node indices
  void Compute(const std::vector<PacketRaw>& _raw_packets,
               const uint16_t& _source_index,
               const SimulationClock& _clock,
               std::vector<Geodetic>* _min_list,
               std::vector<Geodetic>* _max_list,
               std::vector<std::pair<bool, uint16_t>>* _feedback) const;
  /// @brief Adapts thresholds in response to feedback
  /// @param[in] _success Whether the measurement exceeded the threshold
  /// @param[in] _constellation Constellation of the target satellite
  void Regression(const bool& _success, const uint16_t& _constellation);

 private:
  /// @brief Size of correction in regression action
  double regression_step_;
  /// @brief Cloud depth minimum for existence of rain
  double threshold_rain_;
  /// @brief Flag
  bool flag_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_DATA_PROCESSOR_SOURCE_H_
