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

#include "collaborate/packet_forward.h"

#include <cstdint>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace osse {
namespace collaborate {

PacketForward::PacketForward(const std::vector<uint8_t>& _payload)
    : Packet(_payload),
      route_(UnpackRoute(_payload)),
      event_(UnpackEvent(_payload)),
      feedback_(UnpackFeedback(_payload)) {
}

PacketForward::PacketForward(const PacketForward::PartialRoute& _partial_route,
                             const PacketForward::Event& _event,
                             const uint16_t& _feedback)
    : Packet(PackAll(_partial_route, _event, _feedback)),
      route_(EncodeRoute(_partial_route)),
      event_(_event),
      feedback_(_feedback) {
}

std::string PacketForward::ToString() const {
  std::stringstream stream;
  for (auto transfer : route_) {
    if (transfer.first != std::numeric_limits<uint16_t>::max()) {
      stream  << transfer.first << " - " << transfer.second << std::endl;
    }
  }
  stream  << event_.first << " measures at " << event_.second << std::endl;
  return stream.str();
}

PacketForward::Route PacketForward::UnpackRoute(
    const std::vector<uint8_t>& payload) const {
  std::vector<uint8_t> local = payload;
  Route route;
  int index = 0;
  for (auto &transfer : route) {
    transfer.first = UnpackUInt16T(local, index);
    transfer.second = UnpackUInt64T(local, index+2);
    index += kBytesPerTransfer;
  }
  return route;
}


PacketForward::Event PacketForward::UnpackEvent(
    const std::vector<uint8_t>& payload) const {
  std::vector<uint8_t> local = payload;
  Event event;
  event.first = UnpackUInt16T(local, kEventIndex);
  event.second = UnpackUInt64T(local, kEventIndex+2);
  return event;
}

uint16_t PacketForward::UnpackFeedback(
    const std::vector<uint8_t>& payload) const {
  std::vector<uint8_t> local = payload;
  return UnpackUInt16T(local, kFeedbackIndex);
}

PacketForward::Route PacketForward::EncodeRoute(
    const PacketForward::PartialRoute& _partial_route) const {
  Route route;
  int index = 0;
  for (auto transfer : _partial_route) {
    route[index] = transfer;
    ++index;
  }
  while (index < kMaxTransfers) {
    route[index] = std::make_pair(std::numeric_limits<uint16_t>::max(),
                                  std::numeric_limits<uint64_t>::max());
    ++index;
  }
  return route;
}

PacketForward::PartialRoute PacketForward::DecodePartialRoute() {
  auto transfer = route_.begin();
  bool end = false;
  PartialRoute decoded;
  while (transfer != route_.end() && !end) {
    if ((*transfer).first == std::numeric_limits<uint16_t>::max()) {
      end = true;
    } else {
      decoded.push_back(*transfer);
    }
    ++transfer;
  }
  return decoded;
}

std::vector<uint8_t> PacketForward::PackAll(
    const PacketForward::PartialRoute& _partial_route,
    const PacketForward::Event& _event,
    const uint16_t& _feedback) const {
  PartialRoute local_route = _partial_route;
  Event local_event = _event;
  std::vector<uint8_t> payload;
  int index = 0;
  for (auto transfer : local_route) {
    Pack(transfer.first, &payload);
    Pack(transfer.second, &payload);
    ++index;
  }
  while (index < kMaxTransfers) {
    Pack(std::numeric_limits<uint16_t>::max(), &payload);
    Pack(std::numeric_limits<uint64_t>::max(), &payload);
    ++index;
  }
  Pack(local_event.first, &payload);
  Pack(local_event.second, &payload);
  Pack(_feedback, &payload);
  return payload;
}

std::vector<PacketForward> ReadControlBuffer(
    const std::vector<uint8_t>& _buffer) {
  if (_buffer.size() % PacketForward::kPacketForwardSizeBytes != 0) {
    exit(0);
  }
  uint64_t counter = 0;
  std::vector<PacketForward> messages;
  while (counter < _buffer.size()) {
    int end_index = counter + PacketForward::kPacketForwardSizeBytes;
    std::vector<uint8_t> payload(&_buffer[counter], &_buffer[end_index]);
    PacketForward message(payload);
    messages.push_back(message);
    counter += PacketForward::kPacketForwardSizeBytes;
  }
  return messages;
}

}  // namespace collaborate
}  // namespace osse
