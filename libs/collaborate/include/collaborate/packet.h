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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_H_

#include <cstdint>
#include <vector>

namespace osse {
namespace collaborate {

/// @class Packet
/// @brief An abstract container for information
class Packet {
 public:
  /// @brief Default Constructor
  Packet();
  /// @brief Get payload
  /// @returns payload_ Payload
  const std::vector<uint8_t>& payload() const {return payload_;}

 protected:
  /// @brief Default Constructor
  /// @param[in] _payload The payload
  explicit Packet(const std::vector<uint8_t>& _payload);
  /// @brief Inserts a generic value into the payload
  /// @param[in] _value Value
  /// @param[in] _payload Payload
  template <class T>
  void Pack(const T& _value, std::vector<uint8_t>* _payload) const {
    T local = _value;
    uint8_t* start = reinterpret_cast<uint8_t*>(&local);
    _payload->insert(std::end(*_payload), start, start + sizeof(T));
  }
  /// @brief Unpacks a boolean value from the _payload
  /// @param[in] _payload Payload
  /// @param[in] _index Index in the payload vector
  /// @returns Unpacked boolean value
  bool UnpackBool(const std::vector<uint8_t>& _payload,
                  const uint16_t& _index) const;
  /// @brief Unpacks a short unsigned integer from the _payload
  /// @param[in] _payload Payload
  /// @param[in] _index Index in the payload vector
  /// @returns Unpacked short unsigned integer
  uint16_t UnpackUInt16T(const std::vector<uint8_t>& _payload,
                         const uint16_t& _index) const;
  /// @brief Unpacks a long unsigned integer from the payload
  /// @param[in] _payload Payload
  /// @param[in] _index Index in the payload vector
  /// @returns Unpacked long unsigned integer
  uint64_t UnpackUInt64T(const std::vector<uint8_t>& _payload,
                         const uint16_t& _index) const;
  /// @brief Unpacks an integer from the payload
  /// @param[in] _payload Payload
  /// @param[in] _index Index in the payload vector
  /// @returns Unpacked integer
  int UnpackInteger(const std::vector<uint8_t>& _payload,
                    const uint16_t& _index) const;
  /// @brief Unpacks a double from the payload
  /// @param[in] _payload Payload
  /// @param[in] _index Index in the payload vector
  /// @returns Unpacked double
  double UnpackDouble(const std::vector<uint8_t>& _payload,
                      const uint16_t& _index) const;
  /// @brief Data contents (a vector of bytes)
  std::vector<uint8_t> payload_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_PACKET_H_
