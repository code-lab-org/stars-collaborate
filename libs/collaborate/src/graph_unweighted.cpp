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

#include "collaborate/graph_unweighted.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <limits>
#include <set>
#include <string>

#include "collaborate/data_logger.h"
#include "collaborate/graph.h"
#include "collaborate/graph_weighted.h"
#include "collaborate/util.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

GraphUnweighted::GraphUnweighted(DataLogger* _data_log)
    : Graph(_data_log),
      edges_(new Edges{false}) {
}

void GraphUnweighted::Log(const uint16_t& _num_nodes,
                          const uint64_t& _tick) const {
  bool edges[Graph::kSize];
  uint16_t index_1d = 0;
  for (uint16_t r = 0; r < _num_nodes; ++r) {
    for (uint16_t c = 0; c < _num_nodes; ++c) {
      uint64_t index_2d = util::Index(r, c, Graph::kMaxNodes);
      edges[index_1d] = (*edges_)[index_2d];
      ++index_1d;
    }
  }
  data_log_->LogUnweightedGraph(_tick, edges, _num_nodes);
}

void GraphUnweighted::SetEdge(const uint16_t& _row,
                              const uint16_t& _col,
                              const bool& _value) {
  (*edges_)[util::Index(_row, _col, Graph::kMaxNodes)] = _value;
}

bool GraphUnweighted::GetEdge(const uint16_t& _row,
                              const uint16_t& _col) const {
  return (*edges_)[util::Index(_row, _col, Graph::kMaxNodes)];
}

std::set<uint16_t> GraphUnweighted::GetRow(const uint16_t& _row,
                                           const uint16_t& _num_nodes) const {
  std::set<uint16_t> row;
  for (uint16_t col = 0; col < _num_nodes; ++col) {
    if (GetEdge(_row, col)) {
      row.insert(col);
    }
  }
  return row;
}

void GraphUnweighted::Clear() {
  (*edges_).fill(false);
}

void GraphUnweighted::TransferRoute(const std::vector<uint16_t>& _route) {
  for (uint16_t i = 0; i < _route.size() - 1; ++i) {
    SetEdge(_route[i], _route[i+1], true);
  }
}

void GraphUnweighted::TransferWeights(const GraphWeighted& _weighted_graph,
                                      const uint16_t& _num_nodes) {
  // Copy the edges that are not equal to zero
  for (uint16_t r = 0; r < _num_nodes; ++r) {
    for (uint16_t c = 0; c < _num_nodes; ++c) {
      double weight = std::abs(_weighted_graph.GetEdge(r, c));
      SetEdge(r, c, (!(weight < std::numeric_limits<double>::epsilon())));
    }
  }
}

void GraphUnweighted::MaskWeighted(const uint16_t& _num_nodes,
                                   GraphWeighted* _weighted_graph) {
  // Copy the edges that are not equal to zero
  for (uint16_t r = 0; r < _num_nodes; ++r) {
    for (uint16_t c = 0; c < _num_nodes; ++c) {
      if (!GetEdge(r, c)) {
        _weighted_graph->SetEdge(r, c, 0);
      }
    }
  }
}

bool GraphUnweighted::Prim(const GraphWeighted& _weighted,
                           const uint16_t& _num_nodes) {
  int start = 0;
  std::array<double, Graph::kMaxNodes> dist{};
  dist.fill(std::numeric_limits<double>::max());
  dist[start] = 0;
  std::array<bool, Graph::kMaxNodes> unseen;
  unseen.fill(false);
  std::array<int, Graph::kMaxNodes> parent{};
  parent[start] = -1;
  bool connected_ = true;
  for (uint16_t count = 0; count < _num_nodes - 1; ++count) {
    int min_index = -1;
    double min_value = std::numeric_limits<double>::max();
    for (uint16_t node = 0; node < _num_nodes; ++node) {
      if (unseen[node] == false && dist[node] < min_value) {
        min_value = dist[node], min_index = node;
      }
    }
    if (min_index != -1) {
      unseen[min_index] = true;
      for (uint16_t node = 0; node < _num_nodes; ++node) {
        double weight = _weighted.GetEdge(node, min_index);
        if ((weight > 0) && !unseen[node] && (weight < dist[node])) {
          parent[node] = min_index;
          dist[node] = weight;
        }
      }
    } else {
      connected_ = false;
    }
  }
  if (connected_) {
    for (uint16_t i = 1; i < _num_nodes; ++i) {
      SetEdge(i, parent[i], true);
      SetEdge(parent[i], i, true);
    }
  }
  return connected_;
}

}  // namespace collaborate
}  // namespace osse
