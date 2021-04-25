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

#include "collaborate/observing_system.h"

#include <cstdint>
#include <vector>

#include "collaborate/data_logger.h"
#include "collaborate/data_processor.h"
#include "collaborate/event_logger.h"
#include "collaborate/node.h"
#include "collaborate/platform_earth.h"
#include "collaborate/platform_orbit.h"
#include "collaborate/scheduler.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/subsystem_comm.h"
#include "collaborate/subsystem_power.h"
#include "collaborate/subsystem_sensing.h"

namespace osse {
namespace collaborate {

ObservingSystem::ObservingSystem(Sun* _sun,
                                 SimulationClock* _clock,
                                 Scheduler* _scheduler,
                                 EventLogger* _event_log)
    : clock_(_clock),
      scheduler_(_scheduler),
      nodes_(std::vector<Node*>()),
      num_samples_(0),
      event_log_(_event_log),
      sun_(_sun) {
}

ObservingSystem::~ObservingSystem() {
  for (auto node : nodes_) {
    delete node;
  }
  nodes_.clear();
}

void ObservingSystem::LogNodes() const {
  for (auto node : nodes_) {
    node->BufferDataLog();
  }
}

void ObservingSystem::Complete() const {
  for (auto node : nodes_) {
    node->Flush();
  }
}

void ObservingSystem::Launch(const std::vector<PlatformOrbit>& _orbits,
                             const uint16_t& _constellation,
                             const bool& _separate,
                             const SubsystemComm& _comm_if,
                             const SubsystemSensing& _sensing_if,
                             const SubsystemPower& _subsystem_power,
                             DataProcessor* _data_processor,
                             DataLogger* _data_log) {
  uint16_t sat = nodes_.size();
  uint16_t group = _constellation;
  for (auto &orbit : _orbits) {
    //  event_log_->log()->info("[{}] Launch {}", *clock_, orbit.kName());
    Node* node = new Node(orbit.kName(),
                          sat++,
                          group,
                          &orbit,
                          _comm_if,
                          _sensing_if,
                          _subsystem_power,
                          clock_,
                          _data_processor,
                          event_log_,
                          _data_log);
    nodes_.push_back(node);
    if (_separate) {
      group++;
    }
  }
}

void ObservingSystem::Place(const std::vector<PlatformEarth>& _earths,
                            const uint16_t& _constellation,
                            const bool& _separate,
                            const SubsystemComm& _comm_if,
                            const SubsystemSensing& _sensing_if,
                            const SubsystemPower& _subsystem_power,
                            DataProcessor* _data_processor,
                            DataLogger* _data_log) {
  uint16_t sat = nodes_.size();
  uint16_t group = _constellation;
  for (auto &earth : _earths) {
    Node* node = new Node(earth.kName(),
                          sat++,
                          group,
                          &earth,
                          _comm_if,
                          _sensing_if,
                          _subsystem_power,
                          clock_,
                          _data_processor,
                          event_log_,
                          _data_log);
    nodes_.push_back(node);
    if (_separate) {
      group++;
    }
  }
}

}  // namespace collaborate
}  // namespace osse
