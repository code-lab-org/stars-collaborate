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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_ORBITAL_STATE_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_ORBITAL_STATE_H_

#include <vector>

#include "collaborate/geodetic.h"
#include "collaborate/reference_frame.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

/// @class OrbitalState
/// @brief The position and orientation of a node
class OrbitalState {
 public:
  /// @brief Constructor
  /// @param[in] _x_m X-component of position (meters)
  /// @param[in] _y_m Y-component of position (meters)
  /// @param[in] _z_m Z-component of position (meters)
  /// @param[in] _latitude_rad Latitude (radians)
  /// @param[in] _longitude_rad Longitude (radians)
  /// @param[in] _altitude_m Altitude (meters)
  /// @param[in] _dx_m_per_s X-component of velocity (meters per second)
  /// @param[in] _dy_m_per_s Y-component of velocity (meters per second)
  /// @param[in] _dz_m_per_s Z-component of velocity (meters per second)
  /// @param[in] _roll_rad Satellite's roll angle (radians)
  /// @param[in] _pitch_rad Satellite's pitch angle (radians)
  /// @param[in] _yaw_rad Satellite's yaw angle (radians)
  OrbitalState(double _x_m,
               double _y_m,
               double _z_m,
               double _latitude_rad,
               double _longitude_rad,
               double _altitude_m,
               double _dx_m_per_s,
               double _dy_m_per_s,
               double _dz_m_per_s,
               double _roll_rad,
               double _pitch_rad,
               double _yaw_rad);
  /// @brief Calculates the orbit reference_frame
  /// @returns Orbit reference_frame
  /// @details
  /// \f[
  /// \hat{y} = \frac{-\vec{p}\times\vec{v}}
  ///                {\left\lvert-\vec{p}\times\vec{v}\right\rvert}
  /// \f]
  /// \f[ \hat{z} = \frac{-\vec{p}}{\left\lvert-\vec{p}\right\rvert} \f]
  /// \f[ \hat{x} = \hat{y}\times\hat{z} \f]
  ReferenceFrame CalculatePlatformOrbitReferenceFrame() const;
  /// @brief Update the orbital_state
  /// @param[in] _x_m X-component of position (meters)
  /// @param[in] _y_m Y-component of position (meters)
  /// @param[in] _z_m Z-component of position (meters)
  /// @param[in] _latitude_rad Latitude (radians)
  /// @param[in] _longitude_rad Longitude (radians)
  /// @param[in] _altitude_m Altitude (meters)
  /// @param[in] _dx_m_per_s X-component of velocity (meters per second)
  /// @param[in] _dy_m_per_s Y-component of velocity (meters per second)
  /// @param[in] _dz_m_per_s Z-component of velocity (meters per second)
  void Update(double _x_m,
              double _y_m,
              double _z_m,
              double _latitude_rad,
              double _longitude_rad,
              double _altitude_m,
              double _dx_m_per_s,
              double _dy_m_per_s,
              double _dz_m_per_s);
  /// @brief Logs the OrbitalState
  /// @returns A vector of doubles
  std::vector<double> ObtainLog() const;
  /// @brief Logs the OrbitalState Geodetic Coordinates
  /// @returns A vector of doubles
  std::vector<double> ObtainGeodeticLog() const;
  /// @brief Get The position (meters and radians)
  /// @returns position_m_rad_ Position (meters and radians)
  const Vector& position_m_rad() const {return position_m_rad_;}
  /// @brief Get the velocity
  /// @returns velocity_ Velocity (meters per second)
  const Vector& velocity_m_per_s() const {return velocity_m_per_s_;}
  /// @brief Get The geodetic position (radians and meters)
  /// @returns geodetic_rad_m_ Geodetic position (radians and meters)
  const Geodetic& geodetic_rad_m() const {return geodetic_rad_m_;}
  /// @brief Get the orbit_frame
  /// @returns orbit_frame_ Orbit_reference_frame (unit)
  const ReferenceFrame& orbit_frame() const {return orbit_frame_;}
  /// @brief Get the body_frame
  /// @returns body_frame_ Body_reference_frame (unit)
  const ReferenceFrame& body_frame() const {return body_frame_;}

 private:
  /// @brief Position (meters and radians)
  Vector position_m_rad_;
  /// @brief Velocity (meters per second)
  Vector velocity_m_per_s_;
  /// @brief Geodetic position (radians and meters)
  Geodetic geodetic_rad_m_;
  /// @brief PlatformOrbit ReferenceFrame (unit)
  ReferenceFrame orbit_frame_;
  /// @brief Body ReferenceFrame (unit)
  ReferenceFrame body_frame_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_ORBITAL_STATE_H_
