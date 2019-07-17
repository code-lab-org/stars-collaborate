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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_RETURN_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_RETURN_H_

#include <array>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "collaborate/packet.h"

namespace osse {
namespace collaborate {

/// @class PacketReturn
/// @brief A packet of return data
class PacketReturn : public Packet {
 public:
  /// @brief The size (bytes)
  static constexpr int kPacketReturnSizeBytes = 303;
  /// @brief The maximum size of a route
  static constexpr int kMaxTransfers = 30;
  /// @brief The number of bytes per transfer
  static constexpr int kBytesPerTransfer = 10;
  /// @brief The index of the event
  static constexpr int kSuccessIndex = kMaxTransfers * kBytesPerTransfer;
  /// @brief A route containing node and transfer pairs
  typedef std::pair<uint16_t, uint64_t> Event;
  /// @brief A partial route containing node and transfer pairs
  typedef std::vector<Event> PartialRoute;
  /// @brief A route containing node and transfer pairs
  typedef std::array<Event, kMaxTransfers> Route;
  /// @brief Constructor from payload
  /// @param[in] _payload Payload
  explicit PacketReturn(const std::vector<uint8_t>& _payload);
  /// @brief Constructor from data members
  /// @param[in] _partial_route Route
  /// @param[in] _success Whether the threshold was exceeded
  PacketReturn(const PartialRoute& _partial_route,
               const std::pair<bool, uint16_t>& _success);
  /// @brief Converts the packet to a string
  /// @returns Packet as a string
  std::string ToString() const;
  /// @brief Gets route
  /// @returns route_ Route
  const Route& route() const {return route_;}
  /// @brief Gets success
  /// @returns success_ Success
  const std::pair<bool, uint16_t>& success() const {return success_;}
  /// @brief Decodes the route into a standard vector
  /// @returns Route as a vector
  PartialRoute DecodePartialRoute();

 private:
  /// @brief Packs member elements into a payload
  /// @param[in] _partial_route Partial route
  /// @param[in] _success Success
  /// @returns Payload
  std::vector<uint8_t> PackBoth(
      const PartialRoute& _partial_route,
      const std::pair<bool, uint16_t>& _success) const;
  /// @brief Translates a vector to an array
  /// @param[in] _partial_route Route as a vector
  /// @returns Route (array)
  Route EncodeRoute(const PartialRoute& _partial_route) const;
  /// @brief Unpacks the route from the payload
  /// @param[in] _payload Payload
  /// @returns Route (array)
  Route UnpackRoute(const std::vector<uint8_t>& _payload) const;
  /// @brief Unpacks the success from the payload
  /// @param[in] _payload Payload
  /// @returns Success
  std::pair<bool, uint16_t> UnpackSuccess(
      const std::vector<uint8_t>& _payload) const;
  /// @brief Route containing individual transfer
  Route route_;
  /// @brief Whether a measurements exceed the desired threshold
  std::pair<bool, uint16_t> success_;
};

/// @brief Output return packet to a stream
/// @param[in,out] strm stream to output to
/// @param[in] _return_packet
inline std::ostream& operator<<(std::ostream& strm,
                                const PacketReturn& _return_packet) {
  return strm << _return_packet.ToString();
}

/// @fn std::vector<PacketReturn> ReadReturnBuffer(
///         std::vector<uint8_t> _buffer)
/// @brief Obtains a list of return packets from the data buffer
/// @param[in] _data_buffer Data buffer
/// @returns Return packets
std::vector<PacketReturn> ReadReturnBuffer(
    const std::vector<uint8_t>& _buffer);

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_RETURN_H_
