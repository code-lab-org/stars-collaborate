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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_EVENT_LOGGER_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_EVENT_LOGGER_H_

#include <memory>
#include <string>

#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace osse {
namespace collaborate {

/// @class EventLogger
/// @brief An interface to the spdlog logger
class EventLogger {
 public:
  /// @brief Constructor
  /// @param[in] _path File sink path
  explicit EventLogger(const std::string& _path);
  /// @brief Initialize the event logger's behavor
  /// @param[in] _level Base level for logging
  /// @param[in] _console_level Level for console logging
  /// @param[in] _utc Whether to use UTC or local time
  void Initialize(const std::string& _level,
                  const std::string& _console_level,
                  const bool& _utc);
  /// @brief Get the spdlog
  /// @returns log_ Pointer to spdlog
  spdlog::logger* log() {return log_.get();}

 private:
  /// @brief Console sink
  std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_;
  /// @brief File sink
  std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_;
  /// @brief Multi-sink
  std::unique_ptr<spdlog::logger> log_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_EVENT_LOGGER_H_
