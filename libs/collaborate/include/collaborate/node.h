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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_NODE_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_NODE_H_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "collaborate/data_logger.h"
#include "collaborate/data_processor.h"
#include "collaborate/event_logger.h"
#include "collaborate/geodetic.h"
#include "collaborate/orbital_state.h"
#include "collaborate/platform.h"
#include "collaborate/subsystem_power.h"
#include "collaborate/subsystem_comm.h"
#include "collaborate/subsystem_sensing.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

/// @class Node
/// @brief A member of the network (ground, air, or space)
class Node {
 public:
  /// @brief Possible modes of operation
  enum class kMode {Free, Carrying, Sensing};
  /// @brief A geodetic log for a node
  typedef struct PartialLog {
    /// @brief Index
    uint16_t index_;
    /// @brief Constellation
    uint16_t constellation_;
    /// @brief Operation mode
    uint64_t mode_;
    /// @brief Latitude (degrees)
    double latitude_;
    /// @brief Longitude (degrees)
    double longitude_;
    /// @brief Altitude (m)
    double altitude_;
    /// @brief Battery energy stored
    double energy_;
    /// @brief Charging status
    bool charging_;
    /// @brief Solar panel effective area
    double area_;
    /// @brief Number of neighbors
    uint16_t num_neighbors_;
  } PartialLog;
  /// @brief Number of simulation increments in a single data log
  static constexpr int kLogBufferSize = 1000;
  /// @brief A buffer for logged node data
  typedef struct LogBuffer {
    /// @brief Index
    int counter;
    /// @brief Index
    uint16_t index[kLogBufferSize];
    /// @brief Constellation
    uint16_t constellation[kLogBufferSize];
    /// @brief Operation mode
    uint64_t mode[kLogBufferSize];
    /// @brief Latitude (degrees)
    double latitude[kLogBufferSize];
    /// @brief Longitude (degrees)
    double longitude[kLogBufferSize];
    /// @brief Altitude (m)
    double altitude[kLogBufferSize];
    /// @brief Battery energy stored
    double energy[kLogBufferSize];
    /// @brief Charging status
    bool charging[kLogBufferSize];
    /// @brief Solar panel effective area
    double area[kLogBufferSize];
    /// @brief Number of neighbors
    uint16_t num_neighbors[kLogBufferSize];
  } LogBuffer;
  /// @brief Construct a node
  /// @param[in] _name Name
  /// @param[in] _index Index
  /// @param[in] _constellation Which constellation it belongs to
  /// @param[in] _platform Platform
  /// @param[in] _comm_if Communication interface
  /// @param[in] _sensing_if Sensing interface
  /// @param[in] _subsystem_power Power subsystem
  /// @param[in] _clock Simulation clock
  /// @param[in] _data_processor Data processor
  /// @param[in] _event_log Event log
  /// @param[in] _data_log Data log
  Node(const std::string& _name,
       const uint16_t& _index,
       const uint16_t& _constellation,
       const Platform* _platform,
       const SubsystemComm& _comm_if,
       const SubsystemSensing& _sensing_if,
       const SubsystemPower& _subsystem_power,
       const SimulationClock* _clock,
       DataProcessor* _data_processor,
       EventLogger* _event_log,
       DataLogger* _data_log);
  /// @brief Update the orbital_state and antenna reference_frames
  /// @param[in] _offset_s Offset from the current time (seconds)
  /// @param[in] _comm_orient Whether to orient the comm interface
  /// @param[in] _sensing_orient Whether to orient the sensing interface
  /// @param[in] _measure Whether to update the sensing interface
  /// @param[in] _charge Whether to charge the battery
  /// @param[in] _power_update Whether to update the power subsystem
  /// @param[in] _communicate Whether to consider communcation
  void Update(const uint64_t& _offset_s,
              const bool& _comm_orient,
              const bool& _sensing_orient,
              const bool& _measure,
              const bool& _charge,
              const bool& _power_update,
              const bool& _communicate);
  /// @brief Adds a measurement to the list of planned measurements
  /// @param[in] _start_s Start time (seconds)
  /// @param[in] _return_index The index of the informer node
  void PlanMeasurement(const uint64_t& _start_s,
                       const uint16_t& _return_index);
  /// @brief Processes Communication buffer to take action
  void AddressCommBuffer();
  /// @brief Switches the mode of the communication interface
  /// @param[in] _mode Interface node
  void SwitchCommunication(const SubsystemComm::kMode& _mode);
  /// @brief Moves all data from sensing interface to communication interface
  void MoveSensorDataToCommBuffer();
  /// @brief Buffers entire data frame into the data log
  void BufferDataLog();
  /// @brief Buffers remainder data fram into the data log
  void Flush();
  /// @brief Sets the data buffer of the communication interface
  /// @param[in] _comm_buffer The data buffer for the communication interface
  void SetCommBuffer(std::vector<uint8_t> _comm_buffer);
  /// @brief Sets the data buffer of the sensing interface
  /// @param[in] _sensing_buffer The data buffer for the sensing interface
  void SetSensingBuffer(std::vector<uint8_t> _sensing_buffer);
  /// @brief Erases the data buffer of the communication interface
  void EraseCommBuffer();
  /// @brief Erases the data buffer of the sensing interface
  void EraseSensingBuffer();
  /// @brief Gets the data buffer of the communication interface
  /// @returns  Data buffer for the communication interface
  std::vector<uint8_t> GetCommBuffer();
  /// @brief Gets the data buffer of the sensing interface
  /// @returns Data buffer for the sensing interface
  std::vector<uint8_t> GetSensingBuffer();
  /// @brief Set mode of operation
  /// @param[in] _mode Mode of operation
  void set_mode(const kMode& _mode);
  /// @brief Get name
  /// @returns name_ Name
  std::string name() const {return name_;}
  /// @brief Get index
  /// @returns index_ Index
  uint16_t index() const {return index_;}
  /// @brief Get constellation
  /// @returns constellation_ Constellation it belongs to
  uint64_t constellation() const {return constellation_;}
  /// @brief Get orbital_state
  /// @returns orbital_state_ OrbitalState
  const OrbitalState& orbital_state() const {return orbital_state_;}
  /// @brief Get kPlatform
  /// @returns kPlatform_ Platform
  const Platform* kPlatform() const {return kPlatform_;}
  /// @brief Get communication interface
  /// @returns comm_if_ Communication interface
  const SubsystemComm& comm_if() const {return comm_if_;}
  /// @brief Get sensing interface
  /// @returns sensing_if_ Sensing interface
  const SubsystemSensing& sensing_if() const {return sensing_if_;}
  /// @brief Get mode of operation
  /// @returns mode_ Mode of operation
  const kMode& mode() const {return mode_;}
  /// @brief Get power subsystem
  /// @returns subsystem_power_ Power subsystem
  const SubsystemPower& subsystem_power() const {return subsystem_power_;}
  /// @brief Get suggested minimal places to visit
  /// @returns min_suggestions_ Suggested minimal places to visit
  const std::vector<Geodetic>& min_suggestions() const {
    return min_suggestions_;
  }
  /// @brief Get suggested maximal places to visit
  /// @returns max_suggestions_ Suggested maximal places to visit
  const std::vector<Geodetic>& max_suggestions() const {
    return max_suggestions_;
  }
  /// @brief Get feedback
  /// @returns feedback_ feedback
  const std::vector<std::pair<bool, uint16_t>>& feedback() const {
    return feedback_;
  }
  /// @brief Set minimum suggestions
  /// @param[in] _min_suggestions Minimum suggestions
  void set_min_suggestions(const std::vector<Geodetic>& _min_suggestions) {
    min_suggestions_ = _min_suggestions;
  }
  /// @brief Set maximum suggestions
  /// @param[in] _max_suggestions Maximum suggestions
  void set_max_suggestions(const std::vector<Geodetic>& _max_suggestions) {
    max_suggestions_ = _max_suggestions;
  }
  /// @brief Set feedback
  /// @param[in] _feedback feedback
  void set_feedback(const std::vector<std::pair<bool, uint16_t>>& _feedback) {
    feedback_ = _feedback;
  }
  /// @brief Set target index
  /// @param[in] _target_index Target index
  void set_target_index(const int& _target_index) {
    target_index_ = _target_index;
  }
  /// @brief Set number of neighbors
  /// @param[in] _num_neighbors number of neighbors
  void set_num_neighbors(const uint16_t& _num_neighbors) {
    num_neighbors_ = _num_neighbors;
  }
  /// @brief Get target index
  /// @returns target_index_ Target index
  const int& target_index() const {return target_index_;}

 private:
  /// @brief Update orbital_state
  /// @param[in] _offset_s Time offset (seconds)
  void UpdateOrbitalState(const uint64_t& _offset_s);
  /// @brief Update communication antenna orientation
  void UpdateCommAntenna();
  /// @brief Update sensing antenna orientation
  void UpdateSensingAntenna();
  /// @brief Update measurements
  void UpdateMeasurement();
  /// @brief Update power management
  /// @param[in] _charge Whether to charge the battery
  void UpdatePower(const bool& _charge);
  /// @brief Update commmunication interface
  void UpdateCommunication();
  /// @brief Name
  const std::string name_;
  /// @brief Index
  const uint16_t index_;
  /// @brief Constellation
  const uint16_t constellation_;
  /// @brief Platform
  const Platform* kPlatform_;
  /// @brief Data processor
  DataProcessor* kDataProcessor_;
  /// @brief OrbitalState
  OrbitalState orbital_state_;
  /// @brief Communication interface
  SubsystemComm comm_if_;
  /// @brief Sensing interface
  SubsystemSensing sensing_if_;
  /// @brief Mode of operation
  kMode mode_;
  /// @brief Power sybsystem
  SubsystemPower subsystem_power_;
  /// @brief List of planned measurements
  std::vector<std::pair<uint64_t, uint16_t>> measurements_;
  /// @brief List of suggested target minimal locations
  std::vector<Geodetic> min_suggestions_;
  /// @brief List of suggested target maximal locations
  std::vector<Geodetic> max_suggestions_;
  /// @brief Target node index
  int target_index_;
  /// @brief List of informer indices
  std::vector<std::pair<bool, uint16_t>> feedback_;
  /// @brief The number of other visible nodes
  uint16_t num_neighbors_;
  /// @brief Simulation clock
  const SimulationClock* clock_;
  /// @brief Event log
  EventLogger* event_log_;
  /// @brief Buffer for data log
  LogBuffer log_buffer_;
  /// @brief Data log
  DataLogger* data_log_;
  /// @brief Number of completed data buffer logs
  uint64_t num_logs_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_NODE_H_
