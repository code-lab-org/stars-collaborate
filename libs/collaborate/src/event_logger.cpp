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

#include "collaborate/event_logger.h"

#include <memory>
#include <string>

#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace osse {
namespace collaborate {

EventLogger::EventLogger(const std::string& _path)
    : console_(std::make_shared<spdlog::sinks::stdout_color_sink_mt>()),
      file_(std::make_shared<spdlog::sinks::basic_file_sink_mt>(_path, true)),
      log_(new spdlog::logger("collaborate", {console_, file_})) {
}

void EventLogger::Initialize(const std::string& _level,
                             const std::string& _console_level,
                             const bool& _utc) {
  if (_level == "off") {
    log_->set_level(spdlog::level::off);
  } else if (_level == "trace") {
    log_->set_level(spdlog::level::trace);
  } else if (_level == "debug") {
    log_->set_level(spdlog::level::debug);
  } else if (_level == "info") {
    log_->set_level(spdlog::level::info);
  } else if (_level == "warn") {
    log_->set_level(spdlog::level::warn);
  } else if (_level == "error") {
    log_->set_level(spdlog::level::err);
  } else if (_level == "critical") {
    log_->set_level(spdlog::level::critical);
  }
  if (_console_level == "off") {
    console_->set_level(spdlog::level::off);
  } else if (_console_level == "trace") {
    console_->set_level(spdlog::level::trace);
  } else if (_console_level == "debug") {
    console_->set_level(spdlog::level::debug);
  } else if (_console_level == "info") {
    console_->set_level(spdlog::level::info);
  } else if (_console_level == "warn") {
    console_->set_level(spdlog::level::warn);
  } else if (_console_level == "error") {
    console_->set_level(spdlog::level::err);
  } else if (_console_level == "critical") {
    console_->set_level(spdlog::level::critical);
  }
  std::string pattern = "[%Y-%m-%d %H:%M:%S] [%^%L%$] %v";
  if (_utc) {
    log_->set_pattern(pattern, spdlog::pattern_time_type::utc);
  } else {
    log_->set_pattern(pattern);
  }
}

}  // namespace collaborate
}  // namespace osse
