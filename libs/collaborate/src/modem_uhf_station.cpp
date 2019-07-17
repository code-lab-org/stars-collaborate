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

#include "collaborate/modem_uhf_station.h"

#include "collaborate/modem.h"

namespace osse {
namespace collaborate {

ModemUhfStation::ModemUhfStation()
    : Modem("CadetStation",
            0,
            9600,
            2827433388.230814,  // 450 MHz
            0.6662048995,
            0.3,
            2,
            3000000,
            2890265241.3026094,  // 460 MHz
            0.65172218429,
            12) {
}

}  // namespace collaborate
}  // namespace osse
