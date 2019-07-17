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

#ifndef LIBS_SGP4_INCLUDE_SGP4_SOLAR_POSITION_H_
#define LIBS_SGP4_INCLUDE_SGP4_SOLAR_POSITION_H_

#include "sgp4/date_time.h"
#include "sgp4/eci.h"

namespace osse {
namespace sgp4 {

/**
 * @brief Find the position of the sun
 */
class SolarPosition {
 public:
  SolarPosition() {
  }

  /**
   * @brief Find the position
   * @param[in] dt The date time
   * @returns The position
   */
  Eci FindPosition(const DateTime& dt);

 private:
  double Delta_ET(double year) const;
};

}  // namespace sgp4
}  // namespace osse

#endif  // LIBS_SGP4_INCLUDE_SGP4_SOLAR_POSITION_H_
