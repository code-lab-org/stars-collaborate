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

#include "collaborate/scheduler_alpha.h"

#include <cstdint>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "collaborate/channel.h"
#include "collaborate/earth.h"
#include "collaborate/event_logger.h"
#include "collaborate/geodetic.h"
#include "collaborate/graph.h"
#include "collaborate/node.h"
#include "collaborate/packet_forward.h"
#include "collaborate/packet_return.h"
#include "collaborate/scheduler.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/tree.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

SchedulerAlpha::SchedulerAlpha(SimulationClock* _clock) :
    Scheduler(_clock),
    flag_(false) {
}

SchedulerAlpha::SchedulerAlpha(SimulationClock* _clock, const bool& _flag) :
    Scheduler(_clock),
    flag_(_flag) {
}

void SchedulerAlpha::Update(const std::vector<Node*>& _nodes,
                            EventLogger* _logger) {
  nodes_ = _nodes;
  std::vector<Node*> sources;
  std::vector<Node*> constellation_1;
  std::vector<Node*> constellation_2;
  for (auto &node : nodes_) {
    switch (node->constellation()) {
    case (0):
      sources.push_back(node);
      break;
    case (1):
      constellation_1.push_back(node);
      break;
    case (2):
      constellation_2.push_back(node);
      break;
    }
  }
  uint64_t prediction_s;
  Node* next;
  constexpr uint64_t kSizeB = PacketForward::kPacketForwardSizeBytes;
  std::vector<std::pair<uint16_t, uint64_t>> route;
  for (auto &source : sources) {
    uint64_t contact_s = source->comm_if().RequiredTransferDurationS(kSizeB);
    if (source->min_suggestions().size() > 0) {
      next = nullptr;
      prediction_s = 0;
      if (NextVisitor(source->min_suggestions(), 2, &next, &prediction_s)) {
        uint64_t limit_s = (prediction_s -
                            (next->sensing_if().sensor()->kDurationS() / 2));
        _logger->log()->info("[{}] Predicting N{}>...>N{} within {} s",
                             *clock_,
                             source->index(),
                             next->index(),
                             limit_s);
        route = FindRoute(source->index(), next->index(), contact_s, limit_s);
        if (!route.empty()) {
          _logger->log()->info("[{}] Route found: {}",
                               *clock_,
                               GetRouteLog(source->index(), route));
          uint64_t elapsed_s = clock_->elapsed_s() + limit_s;
          PacketForward packet(route,
                               std::make_pair(next->index(), elapsed_s),
                               source->index());
          source->SetCommBuffer(packet.payload());
          source->AddressCommBuffer();
        }
      }
      source->set_min_suggestions({});
    }
    if (source->max_suggestions().size() > 0) {
      //  std::cout << "non-empty" << std::endl;
      next = nullptr;
      prediction_s = 0;
      if (NextVisitor(source->max_suggestions(), 1, &next, &prediction_s)) {
        //  std::cout << "yes visitor" << std::endl;
        uint64_t limit_s = (prediction_s -
                            (next->sensing_if().sensor()->kDurationS() / 2));
        _logger->log()->info("[{}] Predicting N{}>...>N{} within {} s",
                             *clock_,
                             source->index(),
                             next->index(),
                             limit_s);
        route = FindRoute(source->index(), next->index(), contact_s, limit_s);
        if (!route.empty()) {
          _logger->log()->info("[{}] Route found: {}",
                               *clock_,
                               GetRouteLog(source->index(), route));
          uint64_t elapsed_s = clock_->elapsed_s() + limit_s;
          PacketForward packet(route,
                               std::make_pair(next->index(), elapsed_s),
                               source->index());
          source->SetCommBuffer(packet.payload());
          source->AddressCommBuffer();
        } else {
          //  std::cout << "no route" << std::endl;
        }
      } else {
        //  std::cout << "no next visitor" << std::endl;
      }
      source->set_max_suggestions({});
    }
  }
  uint64_t limit_s = 1500;
  for (auto &sink : constellation_1) {
    uint64_t contact_s = sink->comm_if().RequiredTransferDurationS(kSizeB);
    for (auto feedback : sink->feedback()) {
      next = nodes_[feedback.second];
      _logger->log()->info("[{}] Predicting N{}>...>N{} within {} s",
                           *clock_,
                           sink->index(),
                           next->index(),
                           limit_s);
      route = FindRoute(sink->index(), next->index(), contact_s, limit_s);
      if (!route.empty()) {
        _logger->log()->info("[{}] Route found: {}",
                             *clock_,
                             GetRouteLog(sink->index(), route));
        PacketReturn packet(route, std::make_pair(feedback.first,
                                                  sink->constellation()));
        sink->SetCommBuffer(packet.payload());
        sink->AddressCommBuffer();
      }
    }
    sink->set_feedback({});
  }
}

void SchedulerAlpha::PredictChargeChange(Sun* _sun,
                                         Node* _node,
                                         const uint64_t& _limit_s) {
  // Get current state
  uint64_t offset_s = 0;
  bool current = _node->subsystem_power().charging();
  bool next = current;

  // Iterate time
  for (offset_s = 0; offset_s < _limit_s; ++offset_s) {
    // Predict state
    _sun->Update(offset_s);
    _node->Update(offset_s, false, false, false, false, true, false);
    next = _node->subsystem_power().charging();

    // If charge changes, print the current time
    if (current != next) {
      std::cout << offset_s << std::endl;
      current = next;
    }
  }
  // Reset
  _sun->Update(0);
  _node->Update(0, false, false, false, false, true, false);
}

bool SchedulerAlpha::NextVisitor(
    const std::vector<Geodetic>& _destinations_rad_m,
    const uint16_t& _sink_constellation,
    Node** next_visitor_,
    uint64_t* prediction_s_) {
  constexpr uint64_t kMeasurementRadiusM = 50000;
  constexpr uint64_t kStopTimeS = 5000;
  constexpr uint64_t kIntervalS = 1;
  constexpr uint64_t kInitialOffsetS = 300;
  uint64_t wait_interval[Graph::kMaxNodes];
  std::fill(wait_interval, wait_interval + Graph::kMaxNodes, 0);
  std::vector<Node*> sinks;
  for (auto &node : nodes_) {
    if (node->constellation() == _sink_constellation) {
      sinks.push_back(node);
    }
  }
  bool found = false;
  uint64_t offset_s;
  offset_s = kInitialOffsetS;
  double distance_m = 0;
  uint64_t min_position = std::numeric_limits<uint64_t>::max();
  while ((offset_s < kStopTimeS) && !found) {
    uint16_t index = 0;
    for (auto &node : sinks) {
      if (wait_interval[index] == 0) {
        double min_distance_m = std::numeric_limits<double>::max();
        auto destination = _destinations_rad_m.begin();
        uint64_t position = 0;
        while (destination != _destinations_rad_m.end() && !found) {
          distance_m = NodeSensorDistance(node, *destination, offset_s);
          if (distance_m < kMeasurementRadiusM) {
            if (position < min_position) {
              min_position = position;
              *next_visitor_ = node;
              *prediction_s_ = offset_s;
              if (!found) {
                found = true;
              }
            }
          } else {
            if (distance_m < min_distance_m) {
              min_distance_m = distance_m;
            }
            ++destination;
            ++position;
          }
        }
        if (!found) {
          Vector velocity_m_per_s = node->orbital_state().velocity_m_per_s();
          velocity_m_per_s.CompleteCoordinates();
          double speed_m_per_s = velocity_m_per_s.r_m();
          wait_interval[index] = (min_distance_m/speed_m_per_s/kIntervalS);
        }
      } else {
        wait_interval[index]--;
      }
      ++index;
    }
    offset_s += kIntervalS;
  }
  return found;
}

double SchedulerAlpha::NodeSensorDistance(Node* _node,
                                          const Geodetic& _destination_rad_m,
                                          const uint64_t& _offset_s) {
  _node->Update(_offset_s, false, true, false, false, false, false);
  Vector axis = _node->sensing_if().antenna_frame().z_axis();
  Vector position_m_rad = _node->orbital_state().position_m_rad();
  // From intersection with Earth's surface
  Geodetic place_rad_m(position_m_rad, axis, *clock_, _offset_s);
  _node->Update(0, false, true, false, false, false, false);
  return _destination_rad_m.Haversine(place_rad_m);
}

std::vector<std::pair<uint16_t, uint64_t>> SchedulerAlpha::FindRoute(
    const uint16_t& _start_node_index,
    const uint16_t& _end_node_index,
    const uint64_t& _contact_s,
    const uint64_t& _limit_s) {
  bool finished = false;
  Tree::Branch* parent;
  Tree::Branch* child;
  std::array<uint16_t, Graph::kMaxNodes> level;
  level.fill(std::numeric_limits<uint16_t>::max());
  std::array<uint64_t, Graph::kMaxNodes> rcvd;
  rcvd.fill(std::numeric_limits<uint64_t>::max());
  std::set<uint16_t> tx_indices;
  Tree tree(nodes_[_start_node_index], 5, nodes_[_end_node_index]);
  tx_indices.insert(_start_node_index);
  rcvd[_start_node_index] = 0;
  level[_start_node_index] = 0;
  uint64_t s = 0;
  while ((s < (_limit_s-_contact_s)) && (!finished)) {
    std::set<uint16_t> copy_tx_indices = tx_indices;
    for (auto tx : copy_tx_indices) {
      Node* tx_node = nodes_[tx];
      if (level[tx] < tree.height()) {
        std::vector<uint16_t> rx_indices;
        for (uint16_t rx = 0; rx < nodes_.size(); ++rx) {
          if ((tx != rx) && (level[rx] > (level[tx]+1))) {
            rx_indices.push_back(rx);
          }
        }
        std::vector<uint16_t> rx_possible;
        rx_possible = FindGainsFrom(tx, s, rx_indices);
        for (auto rx : rx_possible) {
          Node* rx_node = nodes_[rx];
          uint64_t start_s = Confirm(tx_node, rx_node, _contact_s, s, rcvd[tx]);
          if (start_s < std::numeric_limits<uint64_t>::max()) {
            uint64_t got_s = start_s + _contact_s;
            parent = tree.SearchSpecific(tx_node, rcvd[tx]);
            if (parent != nullptr) {
              child = tree.AddChild(parent, rx_node, got_s);
              if (child != nullptr) {
                level[rx] = child->level;
                if (child->rx_time_s < rcvd[rx]) {
                  rcvd[rx] = child->rx_time_s;
                }
                if (rx == _end_node_index) {
                  uint16_t new_level = level[tx];
                  if (new_level > 0) {
                    --new_level;
                  }
                  tree.set_height(new_level);
                  if (tx == _start_node_index) {
                    finished = true;
                  }
                }
              }
            }
          }
          // If rx has message, start transmitting
          if (rcvd[rx] <= s) {
            tx_indices.insert(rx);
          }
        }
      }
    }
    s += _contact_s;
  }
  SaveTree(_start_node_index, _end_node_index, tree);
  RestoreNodes();
  return MakeRoute(&tree, _end_node_index, _contact_s);
}

std::vector<std::pair<uint16_t, uint64_t>> SchedulerAlpha::MakeRoute(
    Tree* _tree,
    const uint16_t& _end,
    const uint64_t& _contact_s) {
  std::vector<Tree::Branch*> branches;
  branches = _tree->Ancestry(_tree->BreadthFirstSearch(nodes_[_end]));
  std::vector<std::pair<uint16_t, uint64_t>> route;
  for (uint16_t index = 1; index < branches.size(); ++index) {
    uint16_t rx_index = branches[index]->identity->index();
    uint64_t start_time_s = (clock_->elapsed_s() +
                             branches[index]->rx_time_s -
                             _contact_s);
    route.push_back(std::make_pair(rx_index, start_time_s));
  }
  return route;
}

std::vector<uint16_t> SchedulerAlpha::FindGainsFrom(
    const uint16_t& _tx_index,
    const uint64_t& _offset_s,
    const std::vector<uint16_t>& _rxs) {
  std::vector<uint16_t> rx_possible;
  Node* tx_node = nodes_[_tx_index];
  tx_node->Update(_offset_s, true, false, false, false, false, false);
  for (auto rx_index : _rxs) {
    Node* rx_node = nodes_[rx_index];
    rx_node->Update(_offset_s, true, false, false, false, false, false);
    Vector tx_pos = tx_node->orbital_state().position_m_rad();
    Vector rx_pos = rx_node->orbital_state().position_m_rad();
    if (flag_ || earth::Visible(tx_pos, rx_pos)) {
      Channel channel(tx_node, rx_node);
      channel.Update(*clock_, flag_);
      if (channel.open()) {
        rx_possible.push_back(rx_index);
      }
    }
  }
  return rx_possible;
}

uint64_t SchedulerAlpha::Confirm(Node* _tx_node,
                                 Node* _rx_node,
                                 const uint64_t& _duration_s,
                                 const uint64_t& _original_s,
                                 const uint64_t& _lower_limit_s) {
  uint64_t result_s = std::numeric_limits<uint64_t>::max();
  uint64_t s = _original_s;
  Channel channel(_tx_node, _rx_node);
  channel.Update(*clock_, flag_);
  uint64_t earliest_s = 0;
  if (_original_s > _duration_s) {
    earliest_s = _original_s - _duration_s;
    if (earliest_s < _lower_limit_s) {
      earliest_s = _lower_limit_s;
    }
  }
  // Backward
  while (s > earliest_s && channel.open()) {
    _tx_node->Update(s, true, false, false, false, false, false);
    _rx_node->Update(s, true, false, false, false, false, false);
    channel.Update(*clock_, flag_);
    --s;
  }
  // Catch if it went too far
  while (!channel.open()) {
    ++s;
    _tx_node->Update(s, true, false, false, false, false, false);
    _rx_node->Update(s, true, false, false, false, false, false);
    channel.Update(*clock_, flag_);
  }
  uint64_t start_s = s;
  // Foreward
  s += _duration_s;
  _tx_node->Update(s, true, false, false, false, false, false);
  _rx_node->Update(s, true, false, false, false, false, false);
  channel.Update(*clock_, flag_);
  if (channel.open()) {
    result_s = start_s;
  }
  // Restore
  _tx_node->Update(_original_s, true, false, false, false, false, false);
  _rx_node->Update(_original_s, true, false, false, false, false, false);
  return result_s;
}

std::string SchedulerAlpha::GetRouteLog(
    const uint16_t& _start_index,
    const std::vector<std::pair<uint16_t, uint64_t>>& _route) {
  std::stringstream stream;
  stream << "N" << _start_index;
  for (auto &transfer : _route) {
    stream << ">N" << transfer.first;
  }
  return stream.str();
}

}  // namespace collaborate
}  // namespace osse
