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

#include "collaborate/earth.h"

#include <cmath>
#include <iostream>

#include "collaborate/util.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {
namespace earth {

// bool Visible(const Vector& _receiver, const Vector& _transmitter) {
//   (void) _receiver;
//   (void) _transmitter;
//   return true;
// }

bool Visible(const Vector& _receiver, const Vector& _transmitter) {
  // Receiver and Transmitter
  Vector receiver(_receiver.x_m() / kSemiMajorAxisM,
                  _receiver.y_m() / kSemiMajorAxisM,
                  _receiver.z_m() / kSemiMinorAxisM);
  Vector transmitter(_transmitter.x_m() / kSemiMajorAxisM,
                     _transmitter.y_m() / kSemiMajorAxisM,
                     _transmitter.z_m() / kSemiMinorAxisM);
  bool each_direction[2] = {false, false};
  Vector positions[2] = {receiver, transmitter};
  Vector rays[2] = {(transmitter - receiver), (receiver - transmitter)};
  for (int i = 0; i < 2; ++i) {
    // Ray between Receiver and Transmitter
    Vector unit_ray = rays[i].Unit();

    // Calculate quadratic formula
    unit_ray.CompleteCoordinates();
    double A = std::pow(unit_ray.r_m(), 2);  // should be 1
    double B = 2 * positions[i].Dot(unit_ray);
    positions[i].CompleteCoordinates();
    double C = std::pow(positions[i].r_m(), 2) - 1;

    // If all roots imaginary, ray and ellipsoid do not intersect -> LOS exists
    double discriminant = std::pow(B, 2) - 4*A*C;
    if (discriminant < 0) {
      // Solutions are complex - no intersection
      each_direction[i] = true;

    } else {
      // Find first and second intersection
      double t1 = (-B + std::sqrt(discriminant)) / (2*A);
      double t2 = (-B - std::sqrt(discriminant)) / (2*A);

      // If both solutions are negative, then nothing between rx and tx
      if ((t1 < 0) && (t2 < 0)) {
        each_direction[i] = true;
      }
    }
  }
  return (each_direction[0] || each_direction[1]);
}

void SpecularPoint(const Vector& _tx_position_m_rad,
                   const Vector& _tx_velocity_m_per_s,
                   const Vector& _rx_position_m_rad,
                   const Vector& _rx_velocity_m_per_s,
                   Vector* specular_position_m_rad_,
                   Vector* specular_velocity_m_per_s_) {
  (void) _tx_position_m_rad;
  (void) _tx_velocity_m_per_s;
  (void) _rx_position_m_rad;
  (void) _rx_velocity_m_per_s;
  *specular_position_m_rad_ = SpecularPosition(_tx_position_m_rad,
                                               _rx_position_m_rad,
                                               100);
  (void) specular_velocity_m_per_s_;
}

double Radius(const Vector& _position) {
  double latitude_rad = std::asin(_position.z_m()/_position.r_m());
  return (kSemiMajorAxisM
          * std::sqrt((1 - kEccentricity2)
                      / (1 - (kEccentricity2
                              * std::pow(std::cos(latitude_rad), 2)))));
}

Vector SpecularSpherical(const Vector& _rx_position_m_rad,
                         const Vector& _tx_position_m_rad,
                         const double& _radius_m) {
  constexpr double kMinSpecularOffPlane = 0.001;
  constexpr double kMaxSpecularAngleRad = 0.0001745329;
  Vector position;
  Vector tx_unit_m_rad_ = _tx_position_m_rad.Unit();
  Vector rx_unit_m_rad_ = _rx_position_m_rad.Unit();
  double tx_part = _radius_m / _tx_position_m_rad.r_m();
  double rx_part = _radius_m / _rx_position_m_rad.r_m();
  double delta = tx_unit_m_rad_.Dot(rx_unit_m_rad_);
  double gamma = std::acos(delta);
  double epsilon = std::sin(gamma);
  double phi = std::cos(gamma);
  util::Polynomial coefficients = {{(16 - rx_part) / 24 * epsilon, 0},
                                   {((8 - rx_part) * phi - tx_part) / 6, 0},
                                   {(rx_part - 4) * epsilon / 2, 0},
                                   {phi * (rx_part - 2) + tx_part, 0},
                                   {(1 - rx_part) * epsilon, 0}};
  double alpha = util::MinimumRoot(coefficients);
  double zeta = std::sin(gamma - alpha);
  double eta = std::sin(alpha);
  Vector approximate = ((rx_unit_m_rad_*zeta) + (tx_unit_m_rad_*eta))/epsilon;
  approximate = approximate.Unit();

  // Verify vector is in the plane
  Vector check = _rx_position_m_rad.ConstraintToPlane(_tx_position_m_rad,
                                                      approximate);
  check.CompleteCoordinates();

  // Verify angles
  if ((1 - check.r_m()) < kMinSpecularOffPlane) {
    Vector rs_unit = (_rx_position_m_rad - check).Unit();
    double rx_angle = (rs_unit).AngleBetween(check);
    Vector ts_unit = (_tx_position_m_rad - check).Unit();
    double tx_angle = (ts_unit).AngleBetween(check);
    double angle_difference = std::abs(rx_angle - tx_angle);
    if (angle_difference < kMaxSpecularAngleRad) {
      position = check;
    }
  }
  return position;
}

Vector SpecularPosition(const Vector& _tx_position_m_rad,
                        const Vector& _rx_position_m_rad,
                        const double& _radius_init_m) {
  constexpr double kToleranceM = 1.0;
  constexpr int kMaxIterations = 100;
  double correction_m = 100;
  double radius_m = _radius_init_m;
  if (radius_m <= 0) {
    radius_m = Radius(_rx_position_m_rad);
  }
  Vector position_m_rad_ = SpecularSpherical(_rx_position_m_rad,
                                             _tx_position_m_rad,
                                             radius_m);
  //  std::cout << position_m_rad_ << std::endl;
  //  exit(0);
  if (position_m_rad_) {
    int iteration = 0;
    while ((iteration < kMaxIterations) && (correction_m > kToleranceM)) {
      radius_m = Radius(position_m_rad_);
      Vector approximate = SpecularSpherical(_tx_position_m_rad,
                                             _rx_position_m_rad,
                                             radius_m);
      if (approximate) {
        Vector offset_m_rad_ = position_m_rad_ = approximate;
        offset_m_rad_.CompleteCoordinates();
        correction_m = radius_m * offset_m_rad_.r_m();
        position_m_rad_ = approximate;
      }
    }
  }

  // Constrain to Earth's surface
  position_m_rad_ = position_m_rad_ * Radius(position_m_rad_);
  return position_m_rad_;
}

}  // namespace earth
}  // namespace collaborate
}  // namespace osse
