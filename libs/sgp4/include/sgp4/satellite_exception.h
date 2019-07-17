/*
 * Copyright 2013 Daniel Warner <contact@danrw.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBS_SGP4_INCLUDE_SGP4_SATELLITE_EXCEPTION_H_
#define LIBS_SGP4_INCLUDE_SGP4_SATELLITE_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace osse {
namespace sgp4 {

/**
 * @brief The exception that the SGP4 class throws upon an error.
 */
class SatelliteException : public std::runtime_error {
 public:
  /**
   * @brief Constructor
   * @param[in] message The message
   */
  explicit SatelliteException(const char* message)
      : runtime_error(message) {
  }
};

}  // namespace sgp4
}  // namespace osse

#endif  // LIBS_SGP4_INCLUDE_SGP4_SATELLITE_EXCEPTION_H_
