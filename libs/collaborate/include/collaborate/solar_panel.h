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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_SOLAR_PANEL_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_SOLAR_PANEL_H_

#include "collaborate/reference_frame.h"
#include "collaborate/vector.h"
#include "collaborate/sun.h"

namespace osse {
namespace collaborate {

/// @class SolarPanel
/// @brief A solar panel
/// @image html raster/power.png width=500
/// @image latex vector/power.pdf width=\textwidth
class SolarPanel {
 public:
  /// @brief The sun's irradiance (Watts per meter squared)
  static constexpr double kSolarIrradianceWPerM2 = 1332;
  /// @brief Constructor from attitude angles
  /// @param[in] _efficiency_percent Efficiency (percent)
  /// @param[in] _surface_area_m2 Surface area (meters squared)
  /// @param[in] _roll_rad Roll angle to host body frame (radians)
  /// @param[in] _pitch_rad Pitch angle to host body frame (radians)
  /// @param[in] _yaw_rad Yaw angle to host body frame (radians)
  /// @param[in] _sun Sun
  SolarPanel(const double& _efficiency_percent,
             const double& _surface_area_m2,
             const double& _roll_rad,
             const double& _pitch_rad,
             const double& _yaw_rad,
             Sun* _sun);
  /// @brief Obtain the received power in a direction (Watts)
  /// @returns Power received in a specified direction (Watts)
  /// @details
  /// \f[ I = Solar~Irradiance ~~~ (Watt~per~square~meter) \f]
  /// \f[ \sigma = Effective~Area ~~~ (meters~squared) \f]
  /// \f[ \eta = Efficiency ~~~ (percent) \f]
  /// \f[ P = I\eta\sigma ~~~ (Watts)\f]
  double RxPowerW() const;
  /// @brief Updates the attitude frame orientation and effective area
  /// @param[in] _body_frame Host's body frame
  /// @param[in] _orbit_frame Host's orbit frame
  /// @param[in] _position_m_rad Host's position (meters and radians)
  /// @details
  /// \f[ \vec{p}_{sun} = Solar~Position ~~~ (meters) \f]
  /// \f[ \vec{p} = Current~Position ~~~ (meters) \f]
  /// \f[ A = Total~Surface~Area ~~~ (meters~squared) \f]
  /// \f[ \hat{n} = Unit~Normal~Vector,~Attitude~Z-Axis \f]
  /// \f[ \sigma = A\cos{(\angle{(\hat{n},\vec{r}_{p,sun})})} ~~~ (meters) \f]
  void Update(const ReferenceFrame& _body_frame,
              const ReferenceFrame& _orbit_frame,
              const Vector& _position_m_rad);
  /// @brief Get Effective area (meters squared)
  /// @returns effective_area_m2_ Effective area (meters squared)
  const double& effective_area_m2() const {return effective_area_m2_;}

 private:
  /// @brief Efficiency (percent)
  const double kEfficiencyPercent_;
  /// @brief Surface area (meters squared)
  const double kSurfaceAreaM2_;
  /// @brief Roll angle to host body frame (radians)
  const double kRollRad_;
  /// @brief Pitch angle to host body frame (radians)
  const double kPitchRad_;
  /// @brief Yaw angle to host body frame (radians)
  const double kYawRad_;
  /// @brief Sun
  const Sun* kSun_;
  /// @brief Effective area (meters squared)
  double effective_area_m2_;
  /// @brief Attitude frame
  ReferenceFrame attitude_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_SOLAR_PANEL_H_
