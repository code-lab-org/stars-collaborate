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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_DATA_LOGGER_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_DATA_LOGGER_H_

#include <cstdint>
#include <string>
#include <vector>

#include "netcdf/ncFile.h"
#include "netcdf/ncGroup.h"
#include "netcdf/ncVar.h"

namespace osse {
namespace collaborate {

/// @class DataLogger
/// @brief An interface to the netcdf library
class DataLogger {
 public:
  /// @brief Constructor
  /// @param[in] _path File sink path
  explicit DataLogger(const std::string& _path);
  /// @brief Sets up the netcdf file for logging node data
  /// @param[in] _num_nodes Number of nodes
  /// @param[in] _ticks Number of simulation ticks
  void Simulation(const uint16_t& _num_nodes, const uint64_t& _ticks);
  /// @brief Sets up the netcdf file for logging measurement data
  /// @param[in] _ticks Number of simulation ticks
  void Measurement(const uint64_t& _ticks);
  /// @brief Sets up the netcdf file for logging communication parameters
  /// @param[in] _ticks Number of simulation ticks
  void Channel(const uint64_t& _ticks);
  /// @brief Logs a buffer of node data
  /// @param[in] _node_index Index of the source node
  /// @param[in] _variable NetCDF variable name
  /// @param[in] _values Array of values
  /// @param[in] _index Index in NetCDF variable
  /// @param[in] _count Count of elements to transfer
  template <class T>
  void LogParameter(const int& _node_index,
                    const std::string& _variable,
                    const T* _values,
                    const uint64_t& _index,
                    const uint64_t& _count) {
    netCDF::NcGroup group = groups_[_node_index];
    netCDF::NcVar variable = group.getVar(_variable);
    variable.putVar({_index}, {_count}, _values);
  }
  /// @brief Logs a buffer of time series data
  /// @param[in] _variable NetCDF variable name
  /// @param[in] _values Array of values
  /// @param[in] _count Count of elements to transfer
  template <class T>
  void LogSeries(const std::string& _variable,
                 const T* _values,
                 const uint64_t& _count) {
    netCDF::NcVar variable = ncfile_.getVar(_variable);
    variable.putVar({0}, {_count}, _values);
  }
  /// @brief Setup for unweighted network log
  /// @param[in] _num_nodes Number of nodes in network
  /// @param[in] _ticks Number of ticks in simulation
  void UnweightedNetwork(const uint16_t& _num_nodes, const uint64_t& _ticks);
  /// @brief Setup for weighted network log
  /// @param[in] _num_nodes Number of nodes in network
  /// @param[in] _ticks Number of ticks in simulation
  void WeightedNetwork(const uint16_t& _num_nodes, const uint64_t& _ticks);
  /// @brief Logs date and time data
  /// @param[in] _variable NetCDF variable name
  /// @param[in] _values Integer values
  /// @param[in] _index Index in the NetCDF variable
  /// @param[in] _count Count of elements to transfer
  void LogDateTime(const std::string& _variable,
                   const int* _values,
                   const uint64_t& _index,
                   const uint64_t& _count);
  /// @brief Logs antenna gain pattern
  /// @param[in] _theta_ticks Number of theta values
  /// @param[in] _phi_ticks Number of phi values
  /// @param[in] _gain_array Array of gain values
  void LogAntenna(const uint64_t& _theta_ticks,
                  const uint64_t& _phi_ticks,
                  const double* _gain_array);
  /// @brief Logs unweighted graph
  /// @param[in] _tick Tick of the simulation clock
  /// @param[in] _edges Edge flags
  /// @param[in] _num_nodes Number of nodes in network
  void LogUnweightedGraph(const uint64_t& _tick,
                          const bool* _edges,
                          const uint16_t _num_nodes);
  /// @brief Logs weighted graph
  /// @param[in] _tick Tick of the simulation clock
  /// @param[in] _edges Edge weightes
  /// @param[in] _num_nodes Number of nodes in network
  void LogWeightedGraph(const uint64_t& _tick,
                        const double* _edges,
                        const uint16_t _num_nodes);
  /// @brief Logs measurement
  void LogMeasurement();
  /// @brief Logs communication
  void LogCommunication();

 private:
  /// @brief NetCDF file
  netCDF::NcFile ncfile_;
  /// @brief List of NetCDF groups
  std::vector<netCDF::NcGroup> groups_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_DATA_LOGGER_H_
