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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SCHEDULER_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SCHEDULER_H_

#include <cstdint>
#include <utility>
#include <vector>

#include "collaborate/event_logger.h"
#include "collaborate/geodetic.h"
#include "collaborate/graph_unweighted.h"
#include "collaborate/graph_weighted.h"
#include "collaborate/node.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/tree.h"

namespace osse {
namespace collaborate {

/// @class Scheduler
/// @brief Abstract internal simulation processor
class Scheduler {
 public:
  /// @brief Constructor
  /// @param[in] _clock Simulation clock
  explicit Scheduler(SimulationClock* _clock);
  /// @brief Runs the scheduling algorithm
  /// @param[in] _nodes Nodes
  /// @param[in] _event_log Event logger
  virtual void Update(const std::vector<Node*>& _nodes,
                      EventLogger* _event_log) = 0;
  /// @brief Find the next node to visit a location, at future time (seconds)
  /// @param[in] _destinations_rad_m Destinations (meters and radians)
  /// @param[in] _sink_constellation Which constellation to track
  /// @param[out] next_visitor_ Next visitor
  /// @param[out] prediction_s_ Time of the next visit (seconds)
  /// @returns Whether a visitor has been found (if outputs are valid)
  virtual bool NextVisitor(const std::vector<Geodetic>& _destinations_rad_m,
                           const uint16_t& _sink_constellation,
                           Node** next_visitor_,
                           uint64_t* prediction_s_) = 0;
  /// @brief Constructs the most efficient time-dynamic route available
  /// @param[in] _start Start node index
  /// @param[in] _end End node index
  /// @param[in] _contact_s Duration of contact (seconds)
  /// @param[in] _limit_s Expiration time limit (seconds)
  /// @returns Most efficient time-dynamic route available
  virtual std::vector<std::pair<uint16_t, uint64_t>> FindRoute(
      const uint16_t& _start,
      const uint16_t& _end,
      const uint64_t& _contact_s,
      const uint64_t& _limit_s) = 0;
  /// @brief Fills the unweighted graph with LOS connections
  /// @param[in] _unweighted Unweighted graph
  void AllLos(GraphUnweighted* _unweighted);
  /// @brief Fills the weighted graph with distances
  /// @param[in] _weighted Weighted graph
  void AllDist(GraphWeighted* _weighted);

 protected:
  /// @brief Restores all nodes to original orbital_state
  void RestoreNodes();
  /// @brief Saves a tree to a text file
  /// @param[in] _tree Tree
  /// @param[in] _start Start node index
  /// @param[in] _end End node index
  void SaveTree(const uint16_t& _start,
                const uint16_t& _end,
                const Tree& _tree);
  /// @brief Simulation clock
  SimulationClock* clock_;
  /// @brief Current nodes from sensor network
  std::vector<Node*> nodes_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SCHEDULER_H_
