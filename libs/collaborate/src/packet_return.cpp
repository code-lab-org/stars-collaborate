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

#include "collaborate/packet_return.h"

#include <cstdint>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace osse {
namespace collaborate {

PacketReturn::PacketReturn(const std::vector<uint8_t>& _payload)
    : Packet(_payload),
      route_(UnpackRoute(_payload)),
      success_(UnpackSuccess(_payload)) {
}

PacketReturn::PacketReturn(const PacketReturn::PartialRoute& _partial_route,
                           const std::pair<bool, uint16_t>& _success)
    : Packet(PackBoth(_partial_route, _success)),
      route_(EncodeRoute(_partial_route)),
      success_(_success) {
}

std::string PacketReturn::ToString() const {
  std::stringstream stream;
  for (auto transfer : route_) {
    if (transfer.first != std::numeric_limits<uint16_t>::max()) {
      stream  << transfer.first << " - " << transfer.second << std::endl;
    }
  }
  if (success_.first) {
    stream  << "Success: N"
            << success_.second
            << "measurement exceeded threshold"
            << std::endl;
  } else {
    stream  << "Failure: N"
            << success_.second
            << "measurement was below threshold"
            << std::endl;
  }
  return stream.str();
}

PacketReturn::Route PacketReturn::UnpackRoute(
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


std::pair<bool, uint16_t> PacketReturn::UnpackSuccess(
    const std::vector<uint8_t>& payload) const {
  std::vector<uint8_t> local = payload;
  std::pair<bool, uint16_t> success;
  success.first = UnpackBool(local, kSuccessIndex);
  success.second = UnpackUInt16T(local, kSuccessIndex+1);
  return success;
}

PacketReturn::Route PacketReturn::EncodeRoute(
    const PacketReturn::PartialRoute& _partial_route) const {
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

PacketReturn::PartialRoute PacketReturn::DecodePartialRoute() {
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

std::vector<uint8_t> PacketReturn::PackBoth(
    const PacketReturn::PartialRoute& _partial_route,
    const std::pair<bool, uint16_t>& _success) const {
  PacketReturn::PartialRoute local_route = _partial_route;
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
  Pack(_success.first, &payload);
  Pack(_success.second, &payload);
  return payload;
}

std::vector<PacketReturn> ReadReturnBuffer(
    const std::vector<uint8_t>& _buffer) {
  uint64_t counter = 0;
  std::vector<PacketReturn> messages;
  while (counter < _buffer.size()) {
    int end_index = counter + PacketReturn::kPacketReturnSizeBytes;
    std::vector<uint8_t> payload(&_buffer[counter], &_buffer[end_index]);
    PacketReturn message(payload);
    messages.push_back(message);
    counter += PacketReturn::kPacketReturnSizeBytes;
  }
  return messages;
}

}  // namespace collaborate
}  // namespace osse
