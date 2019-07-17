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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_GRAPH_WEIGHTED_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_GRAPH_WEIGHTED_H_

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "collaborate/data_logger.h"
#include "collaborate/graph.h"

namespace osse {
namespace collaborate {

/// @class GraphWeighted
/// @brief Concrete graph with weighted edges
/// @image html raster/weighted.png width=500
/// @image latex vector/weighted.pdf width=\textwidth
/// @details
/// \f[ n_x n_x \in \mathbb{R} \f]
/// \f[
/// {\bf{E}} = Weighted~Edges =
/// \begin{bmatrix}
/// 0 & n_0 n_1 & \dots & n_0 n_{N-1} & n_0 n_N
/// \\ n_1 n_0 & 0 & \dots & n_1 n_{N-1} & n_1 n_N
/// \\ \vdots & \vdots & \ddots & \vdots & \vdots
/// \\ n_{N-1} n_0 & n_{N-1} n_1 & \dots & 0 & n_{N-1} n_N
/// \\ n_N n_0 & n_N n_1 & \dots &  n_N n_{N-1} & 0
/// \end{bmatrix}
/// \f]
class GraphWeighted : public Graph {
 public:
  /// @brief Underlying adjacency attitude_matrix
  typedef std::array<double, Graph::kSize> Edges;
  /// @brief Constructor
  /// @param[in] _data_log Data logger
  explicit GraphWeighted(DataLogger* _data_log);
  /// @brief Sets all edges to 0.0
  void Clear();
  /// @brief Writes the weighted graph to a binary file
  /// @param[in] _num_nodes Number of nodes (rows and columns)
  /// @param[in] _tick The simulation clock tick
  void Log(const uint16_t& _num_nodes, const uint64_t& _tick) const;
  /// @brief Sets the edge at a row and a column to
  /// @param[in] _row Row
  /// @param[in] _col Column
  /// @param[in] _value Value
  void SetEdge(const uint16_t& _row,
               const uint16_t& _col,
               const double& _value);
  /// @brief Gets the edge at a row and a column to
  /// @param[in] _row Row
  /// @param[in] _col Column
  /// @returns Value
  double GetEdge(const uint16_t& _row, const uint16_t& _col) const;
  /// @brief Uses Dijkstra's algorithm to find shortest path between two nodes
  /// @param[in] _start Beginning index
  /// @param[in] _end End index
  /// @returns Shortest path between the nodes
  std::vector<uint16_t> Dijkstra(const uint16_t& _start, const uint16_t& _end);


  /// @brief Weighted edges
  std::unique_ptr<Edges> edges_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_GRAPH_WEIGHTED_H_
