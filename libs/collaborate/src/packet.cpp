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

#include "collaborate/packet.h"

#include <cstdint>
#include <vector>

namespace osse {
namespace collaborate {

Packet::Packet() : payload_(std::vector<uint8_t>()) {
}

Packet::Packet(const std::vector<uint8_t>& _payload) : payload_(_payload) {
}

bool Packet::UnpackBool(const std::vector<uint8_t>& _payload,
                        const uint16_t& _index) const {
  std::vector<uint8_t> local = _payload;
  uint64_t unpacked = *reinterpret_cast<bool *>(&local[_index]);
  return unpacked;
}

uint64_t Packet::UnpackUInt64T(const std::vector<uint8_t>& _payload,
                               const uint16_t& _index) const {
  std::vector<uint8_t> local = _payload;
  uint64_t unpacked = *reinterpret_cast<uint64_t *>(&local[_index]);
  return unpacked;
}

uint16_t Packet::UnpackUInt16T(const std::vector<uint8_t>& _payload,
                               const uint16_t& _index) const {
  std::vector<uint8_t> local = _payload;
  uint16_t unpacked = *reinterpret_cast<uint16_t *>(&local[_index]);
  return unpacked;
}

int Packet::UnpackInteger(const std::vector<uint8_t>& _payload,
                          const uint16_t& _index) const {
  std::vector<uint8_t> local = _payload;
  int unpacked = *reinterpret_cast<int *>(&local[_index]);
  return unpacked;
}

double Packet::UnpackDouble(const std::vector<uint8_t>& _payload,
                            const uint16_t& _index) const {
  std::vector<uint8_t> local = _payload;
  double unpacked = *reinterpret_cast<double *>(&local[_index]);
  return unpacked;
}

}  // namespace collaborate
}  // namespace osse
