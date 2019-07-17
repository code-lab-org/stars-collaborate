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

#ifndef LIBS_SGP4_INCLUDE_SGP4_ORBITAL_ELEMENTS_H_
#define LIBS_SGP4_INCLUDE_SGP4_ORBITAL_ELEMENTS_H_

#include "sgp4/date_time.h"
#include "sgp4/util.h"

namespace osse {
namespace sgp4 {

class Tle;

/**
 * @brief The extracted orbital elements used by the SGP4 propagator.
 */
class OrbitalElements {
 public:
  /**
   * Constructor
   * @param[in] tle The tle
   */
  explicit OrbitalElements(const Tle& tle);

  /**
   * @brief XMO
   * @returns The mean anomaly
   */
  double MeanAnomoly() const {
    return mean_anomoly_;
  }

  /**
   * @brief XNODEO
   * @returns The ascending node
   */
  double AscendingNode() const {
    return ascending_node_;
  }

  /**
   * @brief OMEGAO
   * @returns The argument perigree
   */
  double ArgumentPerigee() const {
    return argument_perigee_;
  }

  /**
   * @brief EO
   * @returns The Eccentricity
   */
  double Eccentricity() const {
    return eccentricity_;
  }

  /**
   * @brief XINCL
   * @returns The inclination
   */
  double Inclination() const {
    return inclination_;
  }

  /**
   * @brief XNO
   * @returns The mean motion
   */
  double MeanMotion() const {
    return mean_motion_;
  }

  /**
   * @brief BSTAR
   * @returns The bstar
   */
  double BStar() const {
    return bstar_;
  }

  /**
   * @brief AODP
   * @returns The revocered semi-major axis
   */
  double RecoveredSemiMajorAxis() const {
    return recovered_semi_major_axis_;
  }

  /**
   * @brief XNODP
   * @returns The recovered mean motion
   */
  double RecoveredMeanMotion() const {
    return recovered_mean_motion_;
  }

  /**
   * @brief PERIGE
   * @returns The perigree
   */
  double Perigee() const {
    return perigee_;
  }

  /**
   * @brief Period in minutes
   * @returns The period (minutes)
   */
  double Period() const {
    return period_;
  }

  /**
   * @brief EPOCH
   * @returns The epoch
   */
  DateTime Epoch() const {
    return epoch_;
  }

 private:
  double mean_anomoly_;
  double ascending_node_;
  double argument_perigee_;
  double eccentricity_;
  double inclination_;
  double mean_motion_;
  double bstar_;
  double recovered_semi_major_axis_;
  double recovered_mean_motion_;
  double perigee_;
  double period_;
  DateTime epoch_;
};

}  // namespace sgp4
}  // namespace osse

#endif  // LIBS_SGP4_INCLUDE_SGP4_ORBITAL_ELEMENTS_H_
