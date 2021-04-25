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

#include "collaborate/data_processor_sink.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "collaborate/data_processor.h"
#include "collaborate/geodetic.h"
#include "collaborate/packet_raw.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

DataProcessorSink::DataProcessorSink() : DataProcessor() {
}

void DataProcessorSink::Compute(
    const std::vector<PacketRaw>& _raw_packets,
    const uint16_t& _source_index,
    const SimulationClock& _clock,
    std::vector<Geodetic>* _min_list,
    std::vector<Geodetic>* _max_list,
    std::vector<std::pair<bool, uint16_t>>* _feedback) const {
  (void) _source_index;
  (void) _clock;
  (void) _min_list;
  (void) _max_list;
  (void) _feedback;
  bool success = false;
  constexpr double rain_threshold = 0.000005;
  constexpr double opt_threshold = 1;
  if (_raw_packets.size() > 0) {
    PacketRaw middle = _raw_packets[static_cast<int>(_raw_packets.size()/2)];
    double measurement = middle.measurement();
    bool is_rain = false;
    if (middle.name().find("PRECTOT") != std::string::npos) {
      is_rain = true;
    }
    success = ((is_rain && (measurement > rain_threshold))
               || (!is_rain && (measurement < opt_threshold)));
    if (success) {
      _feedback->push_back(std::make_pair(true, middle.informer_index()));
    } else {
      _feedback->push_back(std::make_pair(false, middle.informer_index()));
    }
  }
}

void DataProcessorSink::Regression(const bool& _success,
                                   const uint16_t& _constellation) {
  (void) _success;
  (void) _constellation;
}

}  // namespace collaborate
}  // namespace osse
