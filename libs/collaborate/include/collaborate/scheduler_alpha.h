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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SCHEDULER_ALPHA_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SCHEDULER_ALPHA_H_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "collaborate/event_logger.h"
#include "collaborate/geodetic.h"
#include "collaborate/node.h"
#include "collaborate/scheduler.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/tree.h"

namespace osse {
namespace collaborate {

/// @class SchedulerAlpha
/// @brief Concrete internal simulation processor
/// @image html raster/collaborate.png width=500
/// @image latex vector/collaborate.pdf width=\textwidth
class SchedulerAlpha : public Scheduler {
 public:
  /// @brief Constructor
  /// @param[in] _clock Simulation clock
  SchedulerAlpha(SimulationClock* _clock);
  /// @brief Constructor
  /// @param[in] _clock Simulation clock
  /// @param[in] _flag Flag
  SchedulerAlpha(SimulationClock* _clock, const bool& _flag);
  /// @brief Updates the list of nodes
  /// @param[in] _nodes Nodes
  /// @param[in] _event_log Event logger
  void Update(const std::vector<Node*>& _nodes, EventLogger* _event_log);
  /// @brief Predicts the times at which charge status will change
  /// @param[in] _sun Sun
  /// @param[in] _node Subject node
  /// @param[in] _limit_s Time limit (seconds)
  void PredictChargeChange(Sun* _sun, Node* _node, const uint64_t& _limit_s);
  /// @brief Find the next node to visit a location, at future time (seconds)
  /// @param[in] _destinations_rad_m Destinations
  /// @param[in] _sink_constellation Number of the sink constellation
  /// @param[out] next_visitor_ Next visitor
  /// @param[out] prediction_s_ Time of the next visit (seconds)
  /// @returns Whether a visitor has been found (if outputs are valid)
  bool NextVisitor(const std::vector<Geodetic>& _destinations_rad_m,
                   const uint16_t& _sink_constellation,
                   Node** next_visitor_,
                   uint64_t* prediction_s_);
  /// @brief Constructs the most efficient time-dynamic route available
  /// @param[in] _start Start node index
  /// @param[in] _end End node index
  /// @param[in] _contact_s Duration of contact (seconds)
  /// @param[in] _limit_s Expiration time limit (seconds)
  /// @returns Most efficient time-dynamic route available
  std::vector<std::pair<uint16_t, uint64_t>> FindRoute(
      const uint16_t& _start,
      const uint16_t& _end,
      const uint64_t& _contact_s,
      const uint64_t& _limit_s);

 private:
  /// @brief Determines the distance from a sensor's current reading position
  /// @param[in] _node Node
  /// @param[in] _destination_rad_m Geodetic destination
  /// @param[in] _offset_s Time offset from present (seconds)
  /// @returns Distance from a sensor's current reading position
  double NodeSensorDistance(Node* _node,
                            const Geodetic& _destination_rad_m,
                            const uint64_t& _offset_s);
  /// @brief Makes the route from the tree and adds it to the list
  /// @param[in] _tree Tree
  /// @param[in] _end End node index
  /// @param[in] _contact_s Duration of contact (seconds)
  /// @returns Route
  std::vector<std::pair<uint16_t, uint64_t>> MakeRoute(
      Tree* _tree,
      const uint16_t& _end,
      const uint64_t& _contact_s);
  /// @brief Finds the current open channels with neighbors
  /// @param[in] _tx_index The index of the transmitter
  /// @param[in] _offset_s The time offset from current time (seconds)
  /// @param[in] _rxs The receivers to consider
  /// @returns The possible receivers
  std::vector<uint16_t> FindGainsFrom(const uint16_t& _tx_index,
                                      const uint64_t& _offset_s,
                                      const std::vector<uint16_t>& _rxs);
  /// @brief Confirms that the contact lasts as long as the specified duration
  /// @param[in] _tx_node The transmitter
  /// @param[in] _rx_node The receiver
  /// @param[in] _duration_s The duration of contact (seconds)
  /// @param[in] _original_s The starting offset (seconds)
  /// @param[in] _lower_limit_s The lower limit (seconds)
  /// @returns The starting time of contact, or infinity if none found
  uint64_t Confirm(Node* _tx_node,
                   Node* _rx_node,
                   const uint64_t& _duration_s,
                   const uint64_t& _original_s,
                   const uint64_t& _lower_limit_s);
  /// @brief Obtain the string log for the route
  /// @param[in] _start_index Starting node index
  /// @param[in] _route List of individual transfers
  /// @returns String log for the route
  std::string GetRouteLog(
      const uint16_t& _start_index,
      const std::vector<std::pair<uint16_t, uint64_t>>& _route);
  /// @brief Flag
  bool flag_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SCHEDULER_ALPHA_H_
