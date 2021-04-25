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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_GRAPH_UNWEIGHTED_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_GRAPH_UNWEIGHTED_H_

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "collaborate/data_logger.h"
#include "collaborate/graph.h"
#include "collaborate/graph_weighted.h"

namespace osse {
namespace collaborate {

/// @class GraphUnweighted
/// @brief Concrete graph with unweighted edges
/// @image html raster/prim.png width=500
/// @image latex vector/prim.pdf width=\textwidth
/// @details
/// \f[
/// n_x n_x \in \{True,False\}
/// \f]
/// \f[
/// {\bf{E}} = Unweighted~Edges =
/// \begin{bmatrix}
/// 0 & n_0 n_1 & \dots & n_0 n_{N-1} & n_0 n_N
/// \\ n_1 n_0 & 0 & \dots & n_1 n_{N-1} & n_1 n_N
/// \\ \vdots & \vdots & \ddots & \vdots & \vdots
/// \\ n_{N-1} n_0 & n_{N-1} n_1 & \dots & 0 & n_{N-1} n_N
/// \\ n_N n_0 & n_N n_1 & \dots &  n_N n_{N-1} & 0
/// \end{bmatrix}
/// \f]
class GraphUnweighted : public Graph {
 public:
  /// @brief Constructor
  /// @param[in] _data_log Data logger
  explicit GraphUnweighted(DataLogger* _data_log);
  /// @brief Writes unweighted graph to a binary file
  /// @param[in] _num_nodes Number of nodes (rows and columns)
  /// @param[in] _tick The simulation clock tick
  void Log(const uint16_t& _num_nodes, const uint64_t& _tick) const;
  /// @brief Sets the edge at a row and a column to
  /// @param[in] _row Row
  /// @param[in] _col Column
  /// @param[in] _value Value
  void SetEdge(const uint16_t& _row, const uint16_t& _col, const bool& _value);
  /// @brief Gets the edge at a row and a column to
  /// @param[in] _row Row
  /// @param[in] _col Column
  /// @returns Value
  bool GetEdge(const uint16_t& _row, const uint16_t& _col) const;
  /// @brief Gets an entire row from the graph
  /// @param[in] _row Row number
  /// @param[in] _num_nodes Number of nodes
  /// @returns Set of transmitters
  std::set<uint16_t> GetRow(const uint16_t& _row,
                            const uint16_t& _num_nodes) const;
  /// @brief Sets all edges to false
  void Clear();
  /// @brief The route is copied into the graph
  /// @param[in] _route Route
  void TransferRoute(const std::vector<uint16_t>& _route);
  /// @brief Edges with weights are true and edges without are false
  /// @param[in] _weighted_graph Weighted graph
  /// @param[in] _num_nodes Number of nodes
  void TransferWeights(const GraphWeighted& _weighted_graph,
                       const uint16_t& _num_nodes);
  /// @brief Mask the edges of a weighted graph
  /// @param[in] _weighted_graph Weighted graph
  /// @param[in] _num_nodes Number of nodes
  void MaskWeighted(const uint16_t& _num_nodes,
                    GraphWeighted* _weighted_graph);
  /// @brief Uses Prim's algorithm to find the minimum spanning tree in a graph
  /// @param[in] _weighted Weighted graph
  /// @returns connected_ Whether or not the graph is connected
  /// @param[in] _num_nodes Number of nodes (rows and columns)
  bool Prim(const GraphWeighted& _weighted, const uint16_t& _num_nodes);

 private:
  /// @brief Underlying adjacency attitude_matrix
  typedef std::array<bool, Graph::kSize> Edges;
  /// @brief Unweighted edges
  std::unique_ptr<Edges> edges_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_GRAPH_UNWEIGHTED_H_
