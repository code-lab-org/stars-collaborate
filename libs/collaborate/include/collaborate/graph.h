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

#include <cmath>
#include <cstdint>
#include <string>

#include "collaborate/data_logger.h"

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_GRAPH_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_GRAPH_H_

namespace osse {
namespace collaborate {

/// @class Graph
/// @brief Abstract graph
class Graph {
 public:
  /// @brief Maximum number of nodes
  static constexpr uint16_t kMaxNodes = 200;
  /// @brief Maximum size of of a graph
  static constexpr uint16_t kSize = std::pow(kMaxNodes, 2);
  /// @brief Constructor
  /// @param[in] _data_log Data logger
  explicit Graph(DataLogger* _data_log);
  /// @brief Writes graph to a binary file
  /// @param[in] _num_nodes Number of nodes (rows and columns)
  /// @param[in] _tick The simulation clock tick
  virtual void Log(const uint16_t& _num_nodes, const uint64_t& _tick) const = 0;
  /// @brief Clears all edges
  virtual void Clear() = 0;

 protected:
  /// @brief Data logger
  DataLogger* data_log_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_GRAPH_H_
