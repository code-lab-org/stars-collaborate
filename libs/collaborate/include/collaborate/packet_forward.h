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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_FORWARD_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_FORWARD_H_

#include <array>
#include <cstdint>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "collaborate/packet.h"

namespace osse {
namespace collaborate {

/// @class PacketForward
/// @brief A packet of control data
class PacketForward : public Packet {
 public:
  /// @brief The size (bytes)
  static constexpr int kPacketForwardSizeBytes = 312;
  /// @brief The maximum size of a route
  static constexpr int kMaxTransfers = 30;
  /// @brief The number of bytes per transfer
  static constexpr int kBytesPerTransfer = 10;
  /// @brief The index of the event
  static constexpr int kEventIndex = 300;
  /// @brief The index of the feedback
  static constexpr int kFeedbackIndex = 310;
  /// @brief A route containing node and transfer pairs
  typedef std::pair<uint16_t, uint64_t> Event;
  /// @brief A partial route containing node and transfer pairs
  typedef std::vector<Event> PartialRoute;
  /// @brief A route containing node and transfer pairs
  typedef std::array<Event, kMaxTransfers> Route;
  /// @brief Constructor from payload
  /// @param[in] _payload Payload
  explicit PacketForward(const std::vector<uint8_t>& _payload);
  /// @brief Constructor from data members
  /// @param[in] _partial_route Route
  /// @param[in] _event Measurement event
  /// @param[in] _feedback The index of the feedback node
  PacketForward(const PartialRoute& _partial_route,
                const Event& _event,
                const uint16_t& _feedback);
  /// @brief Converts the packet to a string
  /// @returns Packet as a string
  std::string ToString() const;
  /// @brief Gets route
  /// @returns route_ Route
  const Route& route() const {return route_;}
  /// @brief Gets event
  /// @returns event_ Event
  const Event& event() const {return event_;}
  /// @brief Gets feedback
  /// @returns feedback_ Feedback
  const uint16_t& feedback() const {return feedback_;}
  /// @brief Decodes the route into a standard vector
  /// @returns Route as a vector
  PartialRoute DecodePartialRoute();

 private:
  /// @brief Packs member elements into a payload
  /// @param[in] _partial_route Partial route
  /// @param[in] _event Event
  /// @param[in] _feedback Feedback
  /// @returns Payload
  std::vector<uint8_t> PackAll(const PartialRoute& _partial_route,
                               const Event& _event,
                               const uint16_t& _feedback) const;
  /// @brief Translates a vector to an array
  /// @param[in] _partial_route Route as a vector
  /// @returns Route (array)
  Route EncodeRoute(const PartialRoute& _partial_route) const;
  /// @brief Unpacks the route from the payload
  /// @param[in] _payload Payload
  /// @returns Route (array)
  Route UnpackRoute(const std::vector<uint8_t>& _payload) const;
  /// @brief Unpacks the event from the payload
  /// @param[in] _payload Payload
  /// @returns Event
  Event UnpackEvent(const std::vector<uint8_t>& _payload) const;
  /// @brief Unpacks the feedback from the payload
  /// @param[in] _payload Payload
  /// @returns Feedback
  uint16_t UnpackFeedback(const std::vector<uint8_t>& _payload) const;
  /// @brief Route containing individual transfer
  Route route_;
  /// @brief Node index and time of sensor reading
  Event event_;
  /// @brief Feedback node index
  uint16_t feedback_;
};

/// @brief Output control packet to a stream
/// @param[in,out] strm stream to output to
/// @param[in] _control_packet
inline std::ostream& operator<<(std::ostream& strm,
                                const PacketForward& _control_packet) {
  return strm << _control_packet.ToString();
}

/// @fn std::vector<PacketForward> ReadControlBuffer(
///         std::vector<uint8_t> _buffer)
/// @brief Obtains a list of control packets from the data buffer
/// @param[in] _data_buffer Data buffer
/// @returns Control packets
std::vector<PacketForward> ReadControlBuffer(
    const std::vector<uint8_t>& _buffer);

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_FORWARD_H_
