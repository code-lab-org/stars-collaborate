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

#include "collaborate/observing_system_alpha.h"

#include <cstdint>
#include <limits>

#include "collaborate/channel.h"
#include "collaborate/event_logger.h"
#include "collaborate/node.h"
#include "collaborate/observing_system.h"
#include "collaborate/scheduler.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/util.h"

namespace osse {
namespace collaborate {

ObservingSystemAlpha::ObservingSystemAlpha(Sun* _sun,
                                           SimulationClock* _clock,
                                           Scheduler* _scheduler,
                                           EventLogger* _event_log,
                                           DataLogger* _network_log)
    : ObservingSystem(_sun, _clock, _scheduler, _event_log),
      unweighted_(GraphUnweighted(_network_log)),
      channels_(std::vector<Channel>()) {
}

void ObservingSystemAlpha::Seed(const uint64_t& _span_s) {
  for (auto &node : nodes_) {
    uint64_t time_s = util::RandomNumberBetween(0, _span_s/30);
    uint64_t duration_s = node->sensing_if().sensor()->kDurationS();
    while (time_s < (_span_s - duration_s)) {
      node->PlanMeasurement(time_s, std::numeric_limits<uint16_t>::max());
      time_s += duration_s + 50;
      num_samples_++;
    }
  }
  event_log_->log()->info("[{}] Planning {} samples", *clock_, num_samples_);
}

void ObservingSystemAlpha::SeedMany(const uint64_t& _span_s,
                                    const uint16_t& _constellation) {
  std::vector<Node*> sources;
  for (auto &node : nodes_) {
    if (node->constellation() == _constellation) {
      sources.push_back(node);
    }
  }
  for (auto &source : sources) {
    uint64_t time_s = util::RandomNumberBetween(0, _span_s/30);
    uint64_t duration_s = source->sensing_if().sensor()->kDurationS();
    while (time_s < (_span_s - duration_s)) {
      source->PlanMeasurement(time_s, std::numeric_limits<uint16_t>::max());
      time_s += duration_s + 400;
      num_samples_++;
    }
  }
  event_log_->log()->info("[{}] Planning {} samples", *clock_, num_samples_);
}

void ObservingSystemAlpha::Update() {
  event_log_->log()->debug("[{}] incrementing simulation", *clock_);
  constexpr uint64_t kOffsetS = 0;
  sun_->Update(kOffsetS);
  for (auto &node : nodes_) {
    node->Update(kOffsetS, true, true, true, true, true, true);
  }
  event_log_->log()->debug("[{}] scheduling communications", *clock_);
  scheduler_->Update(nodes_, event_log_);
  event_log_->log()->debug("[{}] logging node data", *clock_);
  LogNodes();
  event_log_->log()->debug("[{}] performing data transfers", *clock_);
  ArbitrateCommunication();
}

void ObservingSystemAlpha::Specular() {
  for (auto tx : nodes_) {
    for (auto rx : {nodes_[0]}) {
      if (tx != rx) {
        Vector txp = tx->orbital_state().position_m_rad();
        Vector rxp = rx->orbital_state().position_m_rad();
        Vector sxp = earth::SpecularPosition(txp, rxp, 100);
        if (earth::Visible(txp, rxp)) {
          std::cout << Geodetic(txp, *clock_, 0)
                    << ", "
                    << Geodetic(rxp, *clock_, 0)
                    << ", "
                    << Geodetic(sxp, *clock_, 0)
                    << ", "
                    << std::endl;
        }
      }
    }
  }
}

void ObservingSystemAlpha::LinesOfSight() {
  scheduler_->AllLos(&unweighted_);
  unweighted_.Log(nodes_.size(), clock_->ticks());
}

void ObservingSystemAlpha::ArbitrateCommunication() {
  for (auto &node : nodes_) {
    if (node->target_index() != std::numeric_limits<uint16_t>::max()) {
      Channel channel(node, nodes_[node->target_index()]);
      channel.Update(*clock_);
      channel.Start();
      channels_.push_back(channel);
      unweighted_.SetEdge(node->index(), node->target_index(), true);
    }
  }
  auto channel = channels_.begin();
  while (channel != channels_.end()) {
    (*channel).Update(*clock_);
    bool success = (*channel).success_flag();
    bool error = (*channel).error_flag();
    if (success || error || !(*channel).active()) {
      uint16_t tx_idx = (*channel).tx_node()->index();
      uint16_t rx_idx = (*channel).rx_node()->index();
      unweighted_.SetEdge(tx_idx, rx_idx, false);
      if (success) {
        event_log_->log()->info("[{}] N{}>N{} OK", *clock_, tx_idx, rx_idx);
      } else if (error) {
        event_log_->log()->warn("[{}] N{}>N{} FAIL", *clock_, tx_idx, rx_idx);
      }
      if (!error) {
        (*channel).rx_node()->AddressCommBuffer();
      }
      channels_.erase(channel);
    } else {
      ++channel;
    }
  }
}

}  // namespace collaborate
}  // namespace osse
