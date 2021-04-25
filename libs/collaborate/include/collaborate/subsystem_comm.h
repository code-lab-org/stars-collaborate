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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUBSYSTEM_COMM_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUBSYSTEM_COMM_H_

#include <cstdint>
#include <vector>

#include "collaborate/antenna.h"
#include "collaborate/modem.h"
#include "collaborate/packet_forward.h"
#include "collaborate/packet_return.h"
#include "collaborate/reference_frame.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

/// @class SubsystemComm
/// @brief An interface for RF communication
class SubsystemComm {
 public:
  /// @brief A plan to transfer a packet to another node
  typedef struct CommunicationEvent {
    /// @brief Index of the receiver node
    uint16_t index_;
    /// @brief Total earliest elapsed time when transfer should begin (seconds)
    uint64_t elapsed_s_;
    /// @brief Packet to transfer
    PacketForward packet_;
  } CommunicationEvent;
  /// @brief A plan to feedback a packet to the original node
  typedef struct FeedbackEvent {
    /// @brief Index of the receiver node
    uint16_t index_;
    /// @brief Total earliest elapsed time when transfer should begin (seconds)
    uint64_t elapsed_s_;
    /// @brief Packet to transfer
    PacketReturn packet_;
  } FeedbackEvent;
  /// @brief Possible modes of operation
  enum class kMode {Free, Transmitting, Receiving};
  /// @brief Constructor
  /// @param[in] _antenna Antenna
  /// @param[in] _modem Modem
  SubsystemComm(const Antenna* _antenna, const Modem* _modem);
  /// @brief Update the communication interface
  /// @param[in] _clock Simulation clock
  /// @returns Target receiver index if one has been selected
  uint16_t Update(const SimulationClock& _clock);
  /// @brief Calculate the required transfer duration (seconds)
  /// @returns Required transfer duration (seconds)
  uint64_t RequiredTransferDurationS() const;
  /// @brief Calculate the required transfer duration (seconds)
  /// @returns Required transfer duration (seconds)
  /// @param[in] _buffer_size_bytes Number of bytes in a predicted buffer
  uint64_t RequiredTransferDurationS(const uint64_t& _buffer_size_bytes) const;
  /// @brief Calculate the power consumed during operation
  /// @returns Power consumed during operation
  double CalculatePowerDrainW();
  /// @brief Adds a planned event to storage for later
  /// @param[in] _event Planned commmunication event
  void AddToStorage(CommunicationEvent _event);
  /// @brief Adds a planned event to storage for later
  /// @param[in] _feedback_event Planned feedback event
  void AddToStorage(FeedbackEvent _feedback_event);
  /// @brief Calculates a new attitude frame for the antenna
  /// @param _orbit_frame Satellite orbit frame
  /// @param _body_frame Satellite body frame
  void OrientAntenna(const ReferenceFrame& _orbit_frame,
                     const ReferenceFrame& _body_frame);
  /// @brief Adds data to the data buffer
  /// @param[in] _payload Payload
  void LoadData(const std::vector<uint8_t>& _payload);
  /// @brief Empties data buffer
  void EraseDataBuffer();
  /// @brief Set data buffer
  /// @param[in] _data_buffer Data buffer
  void set_data_buffer(const std::vector<uint8_t>& _data_buffer);
  /// @brief Get Antenna
  /// @returns kAntenna_ Antenna
  const Antenna* kAntenna() const {return kAntenna_;}
  /// @brief Get Antenna reference frame
  /// @returns antenna_frame_ Antenna reference frame
  ReferenceFrame antenna_frame() const {return antenna_frame_;}
  /// @brief Get Data buffer
  /// @returns data_buffer_ Data buffer
  const std::vector<uint8_t>& data_buffer() const {return data_buffer_;}
  /// @brief Activity status
  /// @returns active_ Activity status
  const bool& active() const {return active_;}
  /// @brief Get time counter (seconds)
  /// @returns elapsed_s_ Time counter (seconds)
  const uint64_t& elapsed_s() const {return elapsed_s_;}
  /// @brief Set mode of operation
  /// @param[in] _mode Mode of operation
  void set_mode(const kMode& _mode);
  /// @brief Get mode of operation
  /// @returns mode_ Mode of operation
  const kMode& mode() const {return mode_;}
  /// @brief Get kModem
  /// @returns kModem_ Modem
  const Modem* kModem() const {return kModem_;}
  /// @brief Get list of control packets
  /// @returns storage_ List of control packets
  const std::vector<CommunicationEvent>& storage() {return storage_;}
  /// @brief Get list of return packets
  /// @returns feedback_storage_ List of return packets
  const std::vector<FeedbackEvent>& feedback_storage() {
    return feedback_storage_;
  }

 private:
  /// @brief The modem
  const Modem* kModem_;
  /// @brief List of control packets
  std::vector<CommunicationEvent> storage_;
  /// @brief The mode of operation
  kMode mode_;
  /// @brief List of return packets
  std::vector<FeedbackEvent> feedback_storage_;
  /// @brief Antenna
  const Antenna* kAntenna_;
  /// @brief Antenna reference frame
  ReferenceFrame antenna_frame_;
  /// @brief Data buffer
  std::vector<uint8_t> data_buffer_;
  /// @brief Activity status
  bool active_;
  /// @brief Get time counter (seconds)
  uint64_t elapsed_s_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SUBSYSTEM_COMM_H_
