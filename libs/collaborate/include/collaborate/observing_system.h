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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_OBSERVING_SYSTEM_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_OBSERVING_SYSTEM_H_

#include <cstdint>
#include <vector>

#include "collaborate/data_logger.h"
#include "collaborate/data_processor.h"
#include "collaborate/event_logger.h"
#include "collaborate/node.h"
#include "collaborate/platform_earth.h"
#include "collaborate/platform_orbit.h"
#include "collaborate/subsystem_power.h"
#include "collaborate/subsystem_comm.h"
#include "collaborate/subsystem_sensing.h"
#include "collaborate/scheduler.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

/// @class ObservingSystem
/// @brief Abstract satellite network
/// @image html raster/map.png width=500
/// @image latex vector/map.pdf width=\textwidth
class ObservingSystem {
 public:
  /// @brief Constructor
  /// @param[in] _sun Star at the center of the solar system
  /// @param[in] _clock Simulation clock
  /// @param[in] _scheduler Scheduler
  /// @param[in] _event_log Event log
  ObservingSystem(Sun* _sun,
                  SimulationClock* _clock,
                  Scheduler* _scheduler,
                  EventLogger* _event_log);
  /// @brief Destructor
  ~ObservingSystem();
  /// @brief Generates random list of samples to start with
  /// @param[in] _span_s Total time span of the simulation (seconds)
  /// @param[in] _constellation Constellation
  virtual void SeedMany(const uint64_t& _span_s,
                        const uint16_t& _constellation) = 0;
  /// @brief Update the nodes and protocol
  virtual void Update() = 0;
  /// @brief Make new nodes from a list of platforms
  /// @param[in] _orbits List of orbits
  /// @param[in] _constellation Starting constellation identifier
  /// @param[in] _separate Whether the nodess are separate constellations
  /// @param[in] _comm_if Communication interface
  /// @param[in] _sensing_if Sensing interface
  /// @param[in] _subsystem_power Power subsystem
  /// @param[in] _data_processor Data processor
  /// @param[in] _data_log Data logger
  void Launch(const std::vector<PlatformOrbit>& _orbits,
              const uint16_t& _constellation,
              const bool& _separate,
              const SubsystemComm& _comm_if,
              const SubsystemSensing& _sensing_if,
              const SubsystemPower& _subsystem_power,
              DataProcessor* _data_processor,
              DataLogger* _data_log);
  /// @brief Make new nodes from a list of platforms
  /// @param[in] _earths List of earth platforms
  /// @param[in] _constellation Starting constellation identifier
  /// @param[in] _separate Whether the nodess are separate constellations
  /// @param[in] _comm_if The communication interface
  /// @param[in] _sensing_if Sensing interface
  /// @param[in] _subsystem_power Power subsystem
  /// @param[in] _data_processor Data processor
  /// @param[in] _data_log Data logger
  void Place(const std::vector<PlatformEarth>& _earths,
             const uint16_t& _constellation,
             const bool& _separate,
             const SubsystemComm& _comm_if,
             const SubsystemSensing& _sensing_if,
             const SubsystemPower& _subsystem_power,
             DataProcessor* _data_processor,
             DataLogger* _data_log);
  /// @brief Get the earth_data of nodes
  /// @returns nodes_ The earth_data of nodes
  std::vector<Node*> nodes() const {return nodes_;}
  /// @brief Logs the nodes at the end
  void Complete() const;
  /// @brief Get the scheduler
  /// @returns scheduler_ Scheduler
  Scheduler* scheduler() {return scheduler_;}


 protected:
  /// @brief Logs the nodes
  void LogNodes() const;
  /// @brief SimulationClock
  const SimulationClock* clock_;
  /// @brief Autonomous network scheduler
  Scheduler* scheduler_;
  /// @brief Ordered list of nodes
  std::vector<Node*> nodes_;
  /// @brief Number of samples seeded
  uint64_t num_samples_;
  /// @brief Event logger
  EventLogger* event_log_;
  /// @brief Sun
  Sun* sun_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_OBSERVING_SYSTEM_H_
