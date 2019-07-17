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

#include "collaborate/scheduler.h"

#include <cstdint>
#include <sstream>
#include <vector>

#include "collaborate/channel.h"
#include "collaborate/earth.h"
#include "collaborate/graph_unweighted.h"
#include "collaborate/graph_weighted.h"
#include "collaborate/node.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/tree.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

Scheduler::Scheduler(SimulationClock* _clock)
    : clock_(_clock),
      nodes_(std::vector<Node*>()) {
}

void Scheduler::RestoreNodes() {
  for (auto node : nodes_) {
    node->Update(0, true, true, false, false, true, false);
  }
}

void Scheduler::AllLos(GraphUnweighted* _unweighted) {
  for (uint16_t i = 0; i < nodes_.size(); ++i) {
    for (uint16_t j = 0; j < nodes_.size(); ++j) {
      if (i != j) {
        Vector tx_pos = nodes_[i]->orbital_state().position_m_rad();
        Vector rx_pos = nodes_[j]->orbital_state().position_m_rad();
        _unweighted->SetEdge(i, j, earth::Visible(tx_pos, rx_pos));
      }
    }
  }
}

void Scheduler::AllDist(GraphWeighted* _weighted) {
  for (uint16_t i = 0; i < nodes_.size(); ++i) {
    for (uint16_t j = 0; j < nodes_.size(); ++j) {
      if (i != j) {
        Vector tx_pos = nodes_[i]->orbital_state().position_m_rad();
        Vector rx_pos = nodes_[j]->orbital_state().position_m_rad();
        double value = 0;
        if (earth::Visible(tx_pos, rx_pos)) {
          Vector diff = tx_pos - rx_pos;
          diff.CompleteCoordinates();
          value = diff.r_m();
        }
        _weighted->SetEdge(i, j, value);
      }
    }
  }
}

void Scheduler::SaveTree(const uint16_t& _start,
                         const uint16_t& _end,
                         const Tree& _tree) {
  std::stringstream stream;
  stream << "output/tree_"
         << _start
         << "_"
         << _end
         << "_"
         << clock_->elapsed_s()
         << ".tex";
  _tree.Log(stream.str());
}

}  // namespace collaborate
}  // namespace osse
