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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SIMULATION_CLOCK_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SIMULATION_CLOCK_H_

#include <cstdint>
#include <ostream>
#include <string>

#include "sgp4/date_time.h"

#include "collaborate/data_logger.h"

namespace osse {
namespace collaborate {

/// @class SimulationClock
/// @brief A clock for maintaining the current simulation time
class SimulationClock {
 public:
  /// @brief Size of a log buffer
  static constexpr int kLogBufferSize = 1000;
  /// @brief Log buffer
  typedef struct LogBuffer {
    /// @brief Counter
    int counter;
    /// @brief Year
    int year[kLogBufferSize];
    /// @brief Month
    int month[kLogBufferSize];
    /// @brief Day
    int day[kLogBufferSize];
    /// @brief Hour
    int hour[kLogBufferSize];
    /// @brief Minute
    int minute[kLogBufferSize];
    /// @brief Second
    int second[kLogBufferSize];
    /// @brief Microsecond
    int microsecond[kLogBufferSize];
  } LogBuffer;
  /// @brief Constructor
  /// @param[in] _data_log Data logger
  explicit SimulationClock(DataLogger* _data_log);
  /// @brief Constructor from Year, Month, and Day
  /// @param[in] _data_log Data logger
  /// @param[in] _year Year
  /// @param[in] _month Month
  /// @param[in] _day Day
  SimulationClock(DataLogger* _data_log,
                  const int& _year,
                  const int& _month,
                  const int& _day);
  /// @brief Constructor from YMDHMS
  /// @param[in] _data_log Data logger
  /// @param[in] _year Year
  /// @param[in] _month Month
  /// @param[in] _day Day
  /// @param[in] _hour Hour
  /// @param[in] _minute Minute
  /// @param[in] _second Second
  SimulationClock(DataLogger* _data_log,
                  const int& _year,
                  const int& _month,
                  const int& _day,
                  const int& _hour,
                  const int& _minute,
                  const int& _second);
  /// @brief Increments time
  /// @param[in] _seconds Number of seconds to increment
  void Tick(const uint64_t& _seconds);
  /// @brief Converts current CPU and SGP4 date_time to string
  /// @returns Current CPU and SGP4 date_time as a string
  std::string ToString() const;
  /// @brief Get current date and time
  /// @returns date_time_ Current date and time
  const sgp4::DateTime& date_time() const {return date_time_;}
  /// @brief Get previous time increment (seconds)
  /// @returns last_increment_s_ Previous time increment (seconds)
  const uint64_t& last_increment_s() const {return last_increment_s_;}
  /// @brief Get total time elapsed (seconds)
  /// @returns elapsed_s_ Total time elapsed (seconds)
  const uint64_t& elapsed_s() const {return elapsed_s_;}
  /// @brief Get total ticks
  /// @returns ticks_ Total ticks
  const uint64_t& ticks() const {return ticks_;}
  /// @brief Buffer clock values
  void Buffer();
  /// @brief Write buffers to a log file
  void Flush();

 private:
  /// @brief Current date and time
  sgp4::DateTime date_time_;
  /// @brief Previous time increment (seconds)
  uint64_t last_increment_s_;
  /// @brief Total time elapsed (seconds)
  uint64_t elapsed_s_;
  /// @brief Total ticks
  uint64_t ticks_;
  /// @brief Data logger
  DataLogger* data_log_;
  /// @brief Log buffer
  LogBuffer log_buffer_;
};

/// @brief Output clock to a stream
/// @param[in,out] strm stream to output to
/// @param[in] _simulation_clock The clock
inline std::ostream& operator<<(std::ostream& strm,
                                const SimulationClock& _simulation_clock) {
  return strm << _simulation_clock.ToString();
}


}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SIMULATION_CLOCK_H_
