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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_MODEM_UHF_DEPLOY_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_MODEM_UHF_DEPLOY_H_

#include "collaborate/modem.h"

namespace osse {
namespace collaborate {

/// @class ModemUhfDeploy
/// @brief Concrete UHF modem (deployed on a satellite)
class ModemUhfDeploy : public Modem {
 public:
  /// @brief Constructor
  ModemUhfDeploy();
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_MODEM_UHF_DEPLOY_H_
