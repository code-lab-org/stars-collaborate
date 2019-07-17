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

#include "collaborate/simulation_clock.h"

#include <cstdint>
#include <string>

#include "sgp4/date_time.h"

#include "collaborate/data_logger.h"

namespace osse {
namespace collaborate {

SimulationClock::SimulationClock(DataLogger* _data_log)
    : date_time_(sgp4::DateTime::Now(false)),
      last_increment_s_(0),
      elapsed_s_(0),
      ticks_(0),
      data_log_(_data_log),
      log_buffer_({0, {0}, {0}, {0}, {0}, {0}, {0}, {0}}) {
}

SimulationClock::SimulationClock(DataLogger* _data_log,
                                 const int& _year,
                                 const int& _month,
                                 const int& _day)
    : date_time_(sgp4::DateTime(_year, _month, _day, 0, 0, 0)),
      last_increment_s_(0),
      elapsed_s_(0),
      ticks_(0),
      data_log_(_data_log),
      log_buffer_({0, {0}, {0}, {0}, {0}, {0}, {0}, {0}}) {
}

SimulationClock::SimulationClock(DataLogger* _data_log,
                                 const int& _year,
                                 const int& _month,
                                 const int& _day,
                                 const int& _hour,
                                 const int& _minute,
                                 const int& _second)
    : date_time_(sgp4::DateTime(_year, _month, _day, _hour, _minute, _second)),
      last_increment_s_(0),
      elapsed_s_(0),
      ticks_(0),
      data_log_(_data_log),
      log_buffer_({0, {0}, {0}, {0}, {0}, {0}, {0}, {0}}) {
}

void SimulationClock::Tick(const uint64_t& _seconds) {
  Buffer();
  last_increment_s_ = _seconds;
  elapsed_s_ += _seconds;
  date_time_ = date_time_.AddSeconds(_seconds);
  ticks_ += 1;
}

void SimulationClock::Buffer() {
  if (log_buffer_.counter == kLogBufferSize) {
    Flush();
    log_buffer_.counter = 0;
  }
  int counter = log_buffer_.counter;
  log_buffer_.year[counter] = date_time_.Year();
  log_buffer_.month[counter] = date_time_.Month();
  log_buffer_.day[counter] = date_time_.Day();
  log_buffer_.hour[counter] = date_time_.Hour();
  log_buffer_.minute[counter] = date_time_.Minute();
  log_buffer_.second[counter] = date_time_.Second();
  log_buffer_.microsecond[counter] = date_time_.Microsecond();
  ++log_buffer_.counter;
}

void SimulationClock::Flush() {
  uint64_t count = log_buffer_.counter;
  uint64_t index = ticks_ - count;
  data_log_->LogDateTime("year", log_buffer_.year, index, count);
  data_log_->LogDateTime("month", log_buffer_.month, index, count);
  data_log_->LogDateTime("day", log_buffer_.day, index, count);
  data_log_->LogDateTime("hour", log_buffer_.hour, index, count);
  data_log_->LogDateTime("minute", log_buffer_.minute, index, count);
  data_log_->LogDateTime("second", log_buffer_.second, index, count);
  data_log_->LogDateTime("microsecond", log_buffer_.microsecond, index, count);
}

std::string SimulationClock::ToString() const {
  return date_time().ToString();
}

}  // namespace collaborate
}  // namespace osse
