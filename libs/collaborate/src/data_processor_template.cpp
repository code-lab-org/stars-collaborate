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

#include "collaborate/data_processor_template.h"

#include <cstdint>
#include <utility>
#include <vector>

#include "collaborate/data_processor.h"
#include "collaborate/geodetic.h"
#include "collaborate/packet_raw.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

DataProcessorTemplate::DataProcessorTemplate() : DataProcessor() {
}

void DataProcessorTemplate::Compute(
    const std::vector<PacketRaw>& _raw_packets,
    const uint16_t& _source_index,
    const SimulationClock& _clock,
    std::vector<Geodetic>* _min_list,
    std::vector<Geodetic>* _max_list,
    std::vector<std::pair<bool, uint16_t>>* _feedback) const {
  (void) _raw_packets;
  (void) _source_index;
  (void) _clock;
  (void) _min_list;
  (void) _max_list;
  (void) _feedback;
}

void DataProcessorTemplate::Regression(const bool& _success,
                                       const uint16_t& _constellation) {
  (void) _success;
  (void) _constellation;
}

}  // namespace collaborate
}  // namespace osse
