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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_MODEM_UHF_STATION_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_MODEM_UHF_STATION_H_

#include "collaborate/modem.h"

namespace osse {
namespace collaborate {

/// @class ModemUhfStation
/// @brief Concrete UHF modem (base station)
class ModemUhfStation : public Modem {
 public:
  /// @brief Constructor
  ModemUhfStation();
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_MODEM_UHF_STATION_H_
