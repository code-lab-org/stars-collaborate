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

#include "collaborate/data_logger.h"

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "netcdf/ncDim.h"

namespace osse {
namespace collaborate {

DataLogger::DataLogger(const std::string& _path)
    : ncfile_(_path, netCDF::NcFile::FileMode::replace),
      groups_({}) {
}

void DataLogger::Simulation(const uint16_t& _num_nodes,
                            const uint64_t& _ticks) {
  std::stringstream stream;
  for (uint16_t i = 0; i < _num_nodes; ++i) {
    stream << std::fixed << std::setw(6) << std::setfill('0') << i;
    groups_.push_back(ncfile_.addGroup(stream.str()));
    stream.str("");
  }
  ncfile_.addDim("time", _ticks);
  ncfile_.addVar("year", "int", "time");
  ncfile_.addVar("month", "int", "time");
  ncfile_.addVar("day", "int", "time");
  ncfile_.addVar("hour", "int", "time");
  ncfile_.addVar("minute", "int", "time");
  ncfile_.addVar("second", "int", "time");
  ncfile_.addVar("microsecond", "int", "time");
  for (auto group : groups_) {
    group.addVar("area", "double", "time");
    group.addVar("charging", "byte", "time");
    group.addVar("constellation", "ushort", "time");
    group.addVar("energy", "double", "time");
    group.addVar("index", "ushort", "time");
    group.addVar("latitude", "double", "time");
    group.addVar("longitude", "double", "time");
    group.addVar("mode", "uint64", "time");
    group.addVar("num_neighbors", "ushort", "time");
  }
}

void DataLogger::Measurement(const uint64_t& _ticks) {
  ncfile_.addDim("ticks", _ticks);
  ncfile_.addVar("time", "uint64", "ticks");
  ncfile_.addVar("year", "int", "ticks");
  ncfile_.addVar("month", "int", "ticks");
  ncfile_.addVar("day", "int", "ticks");
  ncfile_.addVar("hour", "int", "ticks");
  ncfile_.addVar("minute", "int", "ticks");
  ncfile_.addVar("second", "int", "ticks");
  ncfile_.addVar("microsecond", "int", "ticks");
  ncfile_.addVar("latitude", "double", "ticks");
  ncfile_.addVar("longitude", "double", "ticks");
  ncfile_.addVar("altitude", "double", "ticks");
  ncfile_.addVar("measurement", "double", "ticks");
  ncfile_.addVar("resolution", "double", "ticks");
  ncfile_.addVar("index", "ushort", "ticks");
}

void DataLogger::Channel(const uint64_t& _ticks) {
  ncfile_.addDim("ticks", _ticks);
  ncfile_.addVar("time", "uint64", "ticks");
  ncfile_.addVar("year", "int", "ticks");
  ncfile_.addVar("month", "int", "ticks");
  ncfile_.addVar("day", "int", "ticks");
  ncfile_.addVar("hour", "int", "ticks");
  ncfile_.addVar("minute", "int", "ticks");
  ncfile_.addVar("second", "int", "ticks");
  ncfile_.addVar("microsecond", "int", "ticks");
  ncfile_.addVar("los_speed", "double", "ticks");
  ncfile_.addVar("omega", "double", "ticks");
  ncfile_.addVar("distance", "double", "ticks");
  ncfile_.addVar("delay", "double", "ticks");
  ncfile_.addVar("data_rate", "double", "ticks");
  ncfile_.addVar("tx_idx", "double", "ticks");
  ncfile_.addVar("tx_buffer", "uint64", "ticks");
  ncfile_.addVar("tx_lon", "double", "ticks");
  ncfile_.addVar("tx_lat", "double", "ticks");
  ncfile_.addVar("tx_alt", "double", "ticks");
  ncfile_.addVar("tx_gain", "double", "ticks");
  ncfile_.addVar("tx_power", "double", "ticks");
  ncfile_.addVar("rx_idx", "double", "ticks");
  ncfile_.addVar("rx_buffer", "uint64", "ticks");
  ncfile_.addVar("rx_lon", "double", "ticks");
  ncfile_.addVar("rx_lat", "double", "ticks");
  ncfile_.addVar("rx_alt", "double", "ticks");
  ncfile_.addVar("rx_gain", "double", "ticks");
  ncfile_.addVar("rx_power", "double", "ticks");
}

void DataLogger::UnweightedNetwork(const uint16_t& _num_nodes,
                                   const uint64_t& _ticks) {
  ncfile_.addDim("time", _ticks);
  ncfile_.addDim("rx", _num_nodes);
  ncfile_.addDim("tx", _num_nodes);
  ncfile_.addVar("edges", "byte", {"rx", "tx", "time"});
}

void DataLogger::WeightedNetwork(const uint16_t& _num_nodes,
                                 const uint64_t& _ticks) {
  ncfile_.addDim("time", _ticks);
  ncfile_.addDim("rx", _num_nodes);
  ncfile_.addDim("tx", _num_nodes);
  ncfile_.addVar("edges", "double", {"rx", "tx", "time"});
}

void DataLogger::LogDateTime(const std::string& _variable,
                             const int* _values,
                             const uint64_t& _index,
                             const uint64_t& _count) {
  netCDF::NcVar variable = ncfile_.getVar(_variable);
  variable.putVar({_index}, {_count}, _values);
}

void DataLogger::LogAntenna(const uint64_t& _theta_ticks,
                            const uint64_t& _phi_ticks,
                            const double* _gain_array) {
  ncfile_.addDim("theta", _theta_ticks);
  ncfile_.addDim("phi", _phi_ticks);
  std::vector<std::string> dims {"theta", "phi"};
  netCDF::NcVar gain = ncfile_.addVar("gain", "double", dims);
  gain.putVar({0, 0}, {_theta_ticks, _phi_ticks}, _gain_array);
}

void DataLogger::LogUnweightedGraph(const uint64_t& _tick,
                                    const bool* _edges,
                                    const uint16_t _num_nodes) {
  netCDF::NcVar edges = ncfile_.getVar("edges");
  edges.putVar({0, 0, _tick}, {_num_nodes, _num_nodes, 1}, _edges);
}

void DataLogger::LogWeightedGraph(const uint64_t& _tick,
                                  const double* _edges,
                                  const uint16_t _num_nodes) {
  netCDF::NcVar edges = ncfile_.getVar("edges");
  edges.putVar({0, 0, _tick}, {_num_nodes, _num_nodes, 1}, _edges);
}

void DataLogger::LogCommunication() {
}

}  // namespace collaborate
}  // namespace osse
