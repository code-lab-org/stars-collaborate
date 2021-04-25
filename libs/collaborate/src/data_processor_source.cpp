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

#include "collaborate/data_processor_source.h"

#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

#include "collaborate/data_processor.h"
#include "collaborate/geodetic.h"
#include "collaborate/packet_raw.h"
#include "collaborate/simulation_clock.h"

namespace osse {
namespace collaborate {

DataProcessorSource::DataProcessorSource()
    : DataProcessor(),
      regression_step_(30),
      threshold_rain_(70),
      flag_(false) {
}

DataProcessorSource::DataProcessorSource(const bool& _flag)
    : DataProcessor(),
      regression_step_(30),
      threshold_rain_(70),
      flag_(_flag) {
}

void DataProcessorSource::Compute(
    const std::vector<PacketRaw>& _raw_packets,
    const uint16_t& _source_index,
    const SimulationClock& _clock,
    std::vector<Geodetic>* _min_list,
    std::vector<Geodetic>* _max_list,
    std::vector<std::pair<bool, uint16_t>>* _feedback) const {
  (void) _source_index;
  (void) _clock;
  (void) _feedback;
  if (flag_) {
    _min_list->push_back(Geodetic(_raw_packets[0].latitude_rad(),
                                  _raw_packets[0].longitude_rad(),
                                  _raw_packets[0].altitude_m()));
    _max_list->push_back(Geodetic(_raw_packets[0].latitude_rad(),
                                  _raw_packets[0].longitude_rad(),
                                  _raw_packets[0].altitude_m()));
  } else {
    constexpr double kOpticalThreshold = 0.1;
    std::vector<std::pair<uint64_t, uint64_t>> indices;
    // bool flag;
    bool flag = false;

    // Minimum
    indices.clear();

    uint64_t marker = 0;
    uint64_t packet_index = 0;
    uint64_t counter = 0;
    for (auto &packet : _raw_packets) {
      if (packet.measurement() < kOpticalThreshold) {
        if (!flag) {
          marker = packet_index;
          flag = true;
          counter = 0;
        } else {
          ++counter;
        }
      } else {
        if (flag)  {
          flag = false;
          indices.push_back(std::make_pair(marker, counter));
        }
      }
      ++packet_index;
    }

    std::pair<uint64_t, uint64_t> pair = std::make_pair(0, 0);
    for (auto &index : indices) {
      if (index.second > pair.second) {
        pair = index;
      }
    }

    std::vector<Geodetic> temp_min_list;
    for (uint64_t i = pair.first; i < (pair.first + pair.second); ++i) {
      temp_min_list.push_back({_raw_packets[i].latitude_rad(),
                               _raw_packets[i].longitude_rad(),
                               _raw_packets[i].altitude_m()});
    }
    uint64_t eighth = temp_min_list.size() / 8;
    uint64_t index2 = 0;
    while (index2 < eighth) {
      _min_list->push_back(temp_min_list[eighth - index2]);
      _min_list->push_back(temp_min_list[eighth + index2]);
      ++index2;
    }

    // Maximum
    indices.clear();
    flag = false;
    marker = 0;
    packet_index = 0;
    counter = 0;
    for (auto &packet : _raw_packets) {
      if (packet.measurement() >= threshold_rain_) {
        if (!flag) {
          marker = packet_index;
          flag = true;
          counter = 0;
        } else {
          ++counter;
        }
      } else {
        if (flag)  {
          flag = false;
          indices.push_back(std::make_pair(marker, counter));
        }
      }
      ++packet_index;
    }
    pair = std::make_pair(0, 0);
    for (auto &index : indices) {
      if (index.second > pair.second) {
        pair = index;
      }
    }
    std::vector<Geodetic> temp_max_list;
    for (uint64_t i = pair.first; i < (pair.first + pair.second); ++i) {
      temp_max_list.push_back({_raw_packets[i].latitude_rad(),
                               _raw_packets[i].longitude_rad(),
                               _raw_packets[i].altitude_m()});
    }
    eighth = temp_max_list.size() / 8;
    index2 = 0;
    while (index2 < eighth) {
      _max_list->push_back(temp_max_list[eighth - index2]);
      _max_list->push_back(temp_max_list[eighth + index2]);
      ++index2;
    }
  }
}

void DataProcessorSource::Regression(const bool& _success,
                                     const uint16_t& _constellation) {
  (void) _success;
  (void) _constellation;
  // if (_constellation == 1) {
  //   if (_success) {
  //     threshold_rain_ -= regression_step_;
  //   } else {
  //     threshold_rain_ += regression_step_;
  //   }
  //   regression_step_ *= 0.92;
  // }
}

}  // namespace collaborate
}  // namespace osse
