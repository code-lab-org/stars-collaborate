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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_OBSERVING_SYSTEM_ALPHA_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_OBSERVING_SYSTEM_ALPHA_H_

#include <cstdint>
#include <string>
#include <vector>

#include "collaborate/channel.h"
#include "collaborate/event_logger.h"
#include "collaborate/graph_unweighted.h"
#include "collaborate/observing_system.h"
#include "collaborate/scheduler.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

/// @class ObservingSystemAlpha
/// @brief Concrete satellite observing system
/// @image html raster/map.png width=500
/// @image latex vector/map.pdf width=\textwidth
class ObservingSystemAlpha : public ObservingSystem {
 public:
  /// @brief Constructor
  /// @param[in] _sun Star at the center of the solar system
  /// @param[in] _clock Simulation clock
  /// @param[in] _collaborate Autonomous network collaborate
  /// @param[in] _event_log Event logger
  /// @param[in] _network_log Network logger
  ObservingSystemAlpha(Sun* _sun,
                       SimulationClock* _clock,
                       Scheduler* _collaborate,
                       EventLogger* _event_log,
                       DataLogger* _network_log);
  /// @brief Generates random list of samples to start with
  /// @param[in] _span_s Total time span of the simulation
  void Seed(const uint64_t& _span_s);
  /// @brief Generates random list of samples to start with for a constellation
  /// @param[in] _span_s Total time span of the simulation
  /// @param[in] _constellation Constellation
  void SeedMany(const uint64_t& _span_s, const uint16_t& _constellation);
  /// @brief Update everything
  void Update();
  /// @brief Calculate all lines of sight between satellites and write log
  void LinesOfSight();

 private:
  /// @brief Creates new channels for nodes needing to communicate
  void ArbitrateCommunication();
  /// @brief Finds all specular points
  void Specular();
  /// @brief Active channels
  GraphUnweighted unweighted_;
  /// @brief An adjacency attitude_matrix
  std::vector<Channel> channels_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_OBSERVING_SYSTEM_ALPHA_H_
