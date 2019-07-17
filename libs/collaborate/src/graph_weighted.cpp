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

#include "collaborate/graph_weighted.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <limits>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "collaborate/data_logger.h"
#include "collaborate/graph.h"
#include "collaborate/util.h"

namespace osse {
namespace collaborate {

GraphWeighted::GraphWeighted(DataLogger* _data_log)
    : Graph(_data_log),
      edges_(new Edges{0}) {
}

void GraphWeighted::Clear() {
  (*edges_).fill(0.0);
}

void GraphWeighted::Log(const uint16_t& _num_nodes,
                        const uint64_t& _tick) const {
  double edges[Graph::kSize];
  uint16_t index_1d = 0;
  for (uint16_t r = 0; r < _num_nodes; ++r) {
    for (uint16_t c = 0; c < _num_nodes; ++c) {
      uint64_t index_2d = util::Index(r, c, Graph::kMaxNodes);
      edges[index_1d] = (*edges_)[index_2d];
      ++index_1d;
    }
  }
  data_log_->LogWeightedGraph(_tick, edges, _num_nodes);
}

void GraphWeighted::SetEdge(const uint16_t& _row,
                            const uint16_t& _col,
                            const double& _value) {
  (*edges_)[util::Index(_row, _col, Graph::kMaxNodes)] = _value;
}

double GraphWeighted::GetEdge(const uint16_t& _row,
                              const uint16_t& _col) const {
  return (*edges_)[util::Index(_row, _col, Graph::kMaxNodes)];
}

std::vector<uint16_t> GraphWeighted::Dijkstra(const uint16_t& start,
                                              const uint16_t& end) {
  // Setup
  std::array<double, Graph::kMaxNodes> dist;
  dist.fill(std::numeric_limits<double>::max());
  dist[start] = 0;
  std::array<uint16_t, Graph::kMaxNodes> parent{};
  std::priority_queue<std::pair<double, uint16_t>,
                      std::vector<std::pair<double, uint16_t> >,
                      std::greater<std::pair<double, uint16_t> > > Q;
  Q.push(std::pair<double, uint16_t>(0, start));
  bool found = false;
  while (!Q.empty()) {
    uint16_t cur_node = Q.top().second;
    double cost_so_far = Q.top().first;
    Q.pop();

    // Skip computation if distance is greater than shortest so far
    if (dist[cur_node] < cost_so_far) continue;

    // Iterate adjacent nodes
    for (uint16_t i = 0; i < Graph::kMaxNodes; ++i) {
      double weight = GetEdge(cur_node, i);
      if (weight > 0) {
        uint16_t adj_node = i;

        // Check for an improvement in path distance
        if (dist[adj_node] > dist[cur_node] + weight) {
          dist[adj_node] = dist[cur_node] + weight;
          Q.push(std::pair<double, uint16_t>(dist[adj_node], adj_node));
          parent[adj_node] = cur_node;
          if (adj_node == end) {
            found = true;
          }
        }
      }
    }
  }

  // Construct path
  uint16_t path_index = end;
  std::vector<uint16_t> path_;
  if (found) {
    while (path_index != start) {
      path_.push_back(path_index);
      path_index = parent[path_index];
    }
  }
  path_.push_back(path_index);
  return path_;
}

}  // namespace collaborate
}  // namespace osse
