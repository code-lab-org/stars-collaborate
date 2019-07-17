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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_EARTH_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_EARTH_H_

#include <cmath>
#include <cstdint>

#include "collaborate/vector.h"

namespace osse {
namespace collaborate {
namespace earth {

/// @file earth.h
/// @brief Provides constants related the the EARTH Earth model

/// @var kFixedCoordinates
/// @brief Whether to convert native ECI coordinates to ECEF
const bool kFixedCoordinates = false;

/// @var kAngularFrequencyRadPerS
/// @brief Earth's rotational speed (radians per second)
constexpr double kAngularFrequencyRadPerS = 0.000072921158553;

/// @var kFlattening
/// @brief Earth's flattening
constexpr double kFlattening = 0.0033528106647474805;

/// @var kEccentricity
/// @brief Earth's flattening
constexpr double kEccentricity = std::sqrt((2 * kFlattening)
                                           - std::pow(kFlattening, 2));

/// @var kEccentricity2
/// @brief Earth's flattening
constexpr double kEccentricity2 = std::pow(kEccentricity, 2);

/// @var kSemiMajorAxisM
/// @brief Earth's semi-major axis (meters)
constexpr double kSemiMajorAxisM = 6378137.0;

/// @var kSemiMinorAxisM
/// @brief Earth's semi-minor axis (meters)
constexpr double kSemiMinorAxisM = 6356752.314245;  // b = a * (1-f)

/// @var kNumLons
/// @brief The number of columns in the earth_data's data array
constexpr uint64_t kNumLons = 5759;

/// @var kNumLats
/// @brief The number of rows in the earth_data's data array
constexpr uint64_t kNumLats = 2880;

/// @var kNumPositions
/// @brief The size of the earth_data
constexpr uint64_t kNumPositions = kNumLons * kNumLats;

/// @var kLonIncrementRad
/// @brief The size of the increment between longitude divisions
constexpr double kLonIncrementRad = ((2 * M_PI) / kNumLons);

/// @var kLatIncrementRad
/// @brief The size of the increment between latitude divisions
constexpr double kLatIncrementRad = (M_PI / kNumLats);

/// @brief Determines if a line-of-sight path exists between two positions
/// @param[in] _subject Position of subject node
/// @param[in] _other Position of other node
/// @returns Whether a line-of-sight path exists
/// @details
/// \f[ \vec{s} = subject~position~vector \f]
/// \f[ \vec{t} = other~position~vector \f]
/// \f[ a = b = EARTH~semimajor~axis,~ c = EARTH~semiminor~axis \f]
/// \f[ \vec{q}=\begin{bmatrix} 1 \div a&1 \div b&1 \div c\end{bmatrix} \f]
/// \f[ \vec{s}_{q} = \vec{s}\vec{q},~\vec{t}_{q} = \vec{t}\vec{q} \f]
/// \f[
/// \vec{r}_{st} = \vec{s}_q - \vec{t}_{q},~
/// \vec{r}_{ts} = \vec{t}_q - \vec{s}_{q}
/// \f]
/// \f[ Discriminant~of~Quadratic~Formula: \f]
/// \f[
/// d_{st} = (2(\vec{s} \cdot \vec{r}_{st}))^2
///          - 4(\left\lvert \vec{r}_{st} \right\rvert^2)
///          * (\left\lvert\vec{s}\right\rvert^2 - 1)
/// \f]
/// \f[
/// d_{ts} = (2(\vec{t} \cdot \vec{r}_{ts}))^2
///          - 4(\left\lvert \vec{r}_{ts} \right\rvert^2)
///          * (\left\lvert\vec{t}\right\rvert^2 - 1)
/// \f]
/// \f[ Visible = (d_{st} > 0) \lor (d_{ts} > 0) \f]
bool Visible(const Vector& _subject, const Vector& _other);

/// @brief Finds a specular point location and velocity
/// @param[in] _tx_position_m_rad Transmitter position (meters and radians)
/// @param[in] _tx_velocity_m_per_s Transmitter velocity (meters per second)
/// @param[in] _rx_position_m_rad Reciever position (meters and radians)
/// @param[in] _rx_velocity_m_per_s Reciever velocity (meters per second)
/// @param[out] specular_position_m_rad_
/// @param[out] specular_velocity_m_per_s_
void SpecularPoint(const Vector& _tx_position_m_rad,
                   const Vector& _tx_velocity_m_per_s,
                   const Vector& _rx_position_m_rad,
                   const Vector& _rx_velocity_m_per_s,
                   Vector* specular_position_m_rad_,
                   Vector* specular_velocity_m_per_s_);

/// @brief Finds the radius of Earth in direction of position
/// @param[in] _position Position
/// @returns Radius of Earth in direction of position
double Radius(const Vector& _position);

/// @brief Finds a specular point on spherical Earth
/// @param[in] _rx_position_m_rad Reciever position (meters and radians)
/// @param[in] _tx_position_m_rad Transmitter position (meters and radians)
/// @param[in] _radius_m Radius (meters)
/// @returns Specular position on spherical Earth
Vector SpecularSpherical(const Vector& _rx_position_m_rad,
                         const Vector& _tx_position_m_rad,
                         const double& _radius_m);

/// @brief Finds a specular point on Earth
/// @param[in] _tx_position_m_rad Transmitter Position (meters and radians)
/// @param[in] _rx_position_m_rad Reciever Position (meters and radians)
/// @param[in] _radius_init_m Initial radius (meters)
/// @returns Specular position on Earth
Vector SpecularPosition(const Vector& _tx_position_m_rad,
                        const Vector& _rx_position_m_rad,
                        const double& _radius_init_m);

}  // namespace earth
}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_EARTH_H_
