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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_GEODETIC_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_GEODETIC_H_

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "collaborate/simulation_clock.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

/// @class Geodetic
/// @brief Latitude longitude, and altitude
class Geodetic {
 public:
  /// @brief Constructor
  Geodetic();
  /// @brief Constructor from LLH
  /// @param[in] _latitude_rad Latitude (radians)
  /// @param[in] _longitude_rad Longitude (radians)
  /// @param[in] _altitude_m Altitude (meters)
  Geodetic(const double& _latitude_rad,
           const double& _longitude_rad,
           const double& _altitude_m);
  /// @brief Constructor from position and current time
  /// @param[in] _position_m_rad Position of node (meters and radians)
  /// @param[in] _clock Simulation clock
  /// @param[in] _offset_s Offset from current time (seconds)
  Geodetic(const Vector& _position_m_rad,
           const SimulationClock& _clock,
           const uint64_t& _offset_s);
  /// @brief Constructor from LLH
  /// @param[in] _triple Array containing latitude, longitude, and altitude
  explicit Geodetic(const std::array<double, 3>& _triple);
  /// @brief Constructor from ray intersection with the Earth
  /// @param[in] _position_m_rad Position of node
  /// @param[in] _direction Direction of the line of sight
  /// @param[in] _clock Simulation clock
  /// @param[in] _offset_s Offset from current time (seconds)
  /// @returns The position of closest intersection (or empty if none)
  /// @details
  /// \f[ \vec{p} = Position \f]
  /// \f[ \hat{r} = Unit~Focus~Direction \f]
  /// \f[ a = b = EARTH~semimajor~axis,~ c = EARTH~semiminor~axis \f]
  /// \f[ \vec{q}_1=\begin{bmatrix} 1 \div a&1 \div b&1 \div c\end{bmatrix} \f]
  /// \f[ \vec{q}_2=\begin{bmatrix} a& b& c\end{bmatrix} \f]
  /// \f[ \vec{p}_{q} = \vec{p}\vec{q}_1 \f]
  /// \f[ Quadratic~Formula \f]
  /// \f[ A = \left\lvert \vec{r} \right\rvert^2 \f]
  /// \f[ B = 2(\vec{p}_{q} \cdot \vec{r}) \f]
  /// \f[ C = \left\lvert\vec{p}_{q}\right\rvert^2 - 1 \f]
  /// \f[ t_1 = \frac{-B+\sqrt{B^2 - 4AC}}{2A} \f]
  /// \f[ t_2 = \frac{-B-\sqrt{B^2 - 4AC}}{2A} \f]
  /// \f[
  /// Intersection =
  /// \min{\left(\left(\vec{p}_{q}-((\vec{p}_{q}+\hat{r}*t_1)*\vec{q}_2)\right),
  /// \left(\vec{p}_{q} - ((\vec{p}_{q}+\hat{r}*t_2)*\vec{q}_2)\right)\right)}
  /// \f]
  Geodetic(const Vector& _position_m_rad,
           const Vector& _direction,
           const SimulationClock& _clock,
           const uint64_t& _offset_s);
  /// @brief Calculates ray intersection with Earth's surface
  /// @param[in] _position_m_rad Position of node
  /// @param[in] _direction Direction of the line of sight
  /// @param[in] _clock Simulation clock
  /// @param[in] _offset_s Offset from current time (seconds)
  /// @returns The position of closest intersection (or empty if none)
  /// @details
  /// \f[ \vec{p} = Position \f]
  /// \f[ \hat{r} = Unit~Focus~Direction \f]
  /// \f[ a = b = EARTH~semimajor~axis,~ c = EARTH~semiminor~axis \f]
  /// \f[ \vec{q}_1=\begin{bmatrix} 1 \div a&1 \div b&1 \div c\end{bmatrix} \f]
  /// \f[ \vec{q}_2=\begin{bmatrix} a& b& c\end{bmatrix} \f]
  /// \f[ \vec{p}_{q} = \vec{p}\vec{q}_1 \f]
  /// \f[ Quadratic~Formula \f]
  /// \f[ A = \left\lvert \vec{r} \right\rvert^2 \f]
  /// \f[ B = 2(\vec{p}_{q} \cdot \vec{r}) \f]
  /// \f[ C = \left\lvert\vec{p}_{q}\right\rvert^2 - 1 \f]
  /// \f[ t_1 = \frac{-B+\sqrt{B^2 - 4AC}}{2A} \f]
  /// \f[ t_2 = \frac{-B-\sqrt{B^2 - 4AC}}{2A} \f]
  /// \f[
  /// Intersection =
  /// \min{\left(\left(\vec{p}_{q}-((\vec{p}_{q}+\hat{r}*t_1)*\vec{q}_2)\right),
  /// \left(\vec{p}_{q} - ((\vec{p}_{q}+\hat{r}*t_2)*\vec{q}_2)\right)\right)}
  /// \f]
  std::array<double, 3>Intersection(const Vector& _position_m_rad,
                                    const Vector& _direction,
                                    const SimulationClock& _clock,
                                    const uint64_t& _offset_s);
  /// @brief Calculates a great-circle distance to another location
  /// @param[in] _other Other geodetic location
  /// @returns distance Great circle distance
  /// @details
  /// \f[
  /// a = EARTH~Semimajor~Axis
  /// \f]
  /// \f[
  /// d = 2a
  /// \arcsin{\sqrt{\left(\sin{\left(\frac{lat_1 - lat_2}{2}\right)}\right)^2
  ///          + \left(\left(\sin{\left(\frac{lon_1 - lon_2}{2}\right)}\right)^2
  ///          * \cos(lat_1*lat_2)\right)}}
  /// \f]
  double Haversine(const Geodetic& _other) const;
  /// @brief Converts to a vector
  /// @param[in] _clock Simulation clock
  /// @returns The vector
  Vector ToVector(const SimulationClock& _clock) const;
  /// @brief Outputs the geodetic to a string
  /// @returns Geodetic as a string
  std::string ToString() const;
  /// @brief Generates a log of coordinates
  /// @returns A log of coordinates
  std::vector<double> ObtainLog() const;
  /// @brief Get latitude (radians)
  /// @returns latitude_rad_ Latitude (radians)
  const double& latitude_rad() const {return latitude_rad_;}
  /// @brief Get longitude (radians)
  /// @returns longitude_rad_ Longitude (radians)
  const double& longitude_rad() const {return longitude_rad_;}
  /// @brief Get altitude (meters)
  /// @returns altitude_m_ Altitude (meters)
  const double& altitude_m() const {return altitude_m_;}

 private:
  /// @brief Calculate latitude from position and current time
  /// @param[in] _position_m_rad Position of node (meters and radians)
  /// @param[in] _clock Simulation clock
  /// @param[in] _offset_s Offset from current time (seconds)
  /// @returns The latitude (radians)
  double CalculateLatitudeRad(const Vector& _position_m_rad,
                              const SimulationClock& _clock,
                              const uint64_t& _offset_s) const;
  /// @brief Calculate longitude from position and current time
  /// @param[in] _position_m_rad Position of node (meters and radians)
  /// @param[in] _clock Simulation clock
  /// @param[in] _offset_s Offset from current time (seconds)
  /// @returns The longitude (radians)
  double CalculateLongitudeRad(const Vector& _position_m_rad,
                               const SimulationClock& _clock,
                               const uint64_t& _offset_s) const;
  /// @brief Calculate alttitude from position and current time
  /// @param[in] _position_m_rad Position of node (meters and radians)
  /// @param[in] _clock Simulation clock
  /// @param[in] _offset_s Offset from current time (seconds)
  /// @returns The altitude (meters)
  double CalculateAltitudeM(const Vector& _position_m_rad,
                            const SimulationClock& _clock,
                            const uint64_t& _offset_s) const;
  /// @brief Latitude (radians)
  double latitude_rad_;
  /// @brief Longitude (radians)
  double longitude_rad_;
  /// @brief Altitude (meters)
  double altitude_m_;
};

/// Output geodetic to a stream
/// @param[in,out] strm stream to output to
/// @param[in] _geodetic Geodetic
inline std::ostream& operator<<(std::ostream& strm, const Geodetic& _geodetic) {
  return strm << _geodetic.ToString();
}

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_GEODETIC_H_
