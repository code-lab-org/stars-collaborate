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

#include "collaborate/geodetic.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

#include "sgp4/coord_geodetic.h"
#include "sgp4/eci.h"
#include "sgp4/vector.h"

#include "collaborate/earth.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

Geodetic::Geodetic() : latitude_rad_(0), longitude_rad_(0), altitude_m_(0) {
}

Geodetic::Geodetic(const double& _latitude_rad,
                   const double& _longitude_rad,
                   const double& _altitude_m)
    : latitude_rad_(_latitude_rad),
      longitude_rad_(_longitude_rad),
      altitude_m_(_altitude_m) {
}

Geodetic::Geodetic(const Vector& _position_m_rad,
                   const SimulationClock& _simulation_clock,
                   const uint64_t& _offset_s)
    : latitude_rad_(CalculateLatitudeRad(_position_m_rad,
                                         _simulation_clock,
                                         _offset_s)),
      longitude_rad_(CalculateLongitudeRad(_position_m_rad,
                                           _simulation_clock,
                                           _offset_s)),
      altitude_m_(CalculateAltitudeM(_position_m_rad,
                                     _simulation_clock,
                                     _offset_s)) {
}

Geodetic::Geodetic(const std::array<double, 3>& _triple)
    : latitude_rad_(_triple[0]),
      longitude_rad_(_triple[1]),
      altitude_m_(_triple[2]) {
}

Geodetic::Geodetic(const Vector& _position_m_rad,
                   const Vector& _direction,
                   const SimulationClock& _simulation_clock,
                   const uint64_t& _offset_s)
    : Geodetic(Intersection(_position_m_rad,
                            _direction,
                            _simulation_clock,
                            _offset_s)) {
}

double Geodetic::Haversine(const Geodetic& _other) const {
  double u = std::sin((latitude_rad_ - _other.latitude_rad())/2);
  double v = std::sin((longitude_rad_ - _other.longitude_rad())/2);
  double w = std::asin(sqrt((u * u)
                            + ((std::cos(_other.latitude_rad())
                                * std::cos(latitude_rad_))
                               * (v * v))));
  return (2.0 * earth::kSemiMajorAxisM * w);
}

Vector Geodetic::ToVector(const SimulationClock& _simulation_clock) const {
  sgp4::CoordGeodetic geo(latitude_rad_, longitude_rad_, altitude_m_, true);
  sgp4::Eci eci(_simulation_clock.date_time(), geo);
  return Vector(eci.Position().x * 1000,
                eci.Position().y * 1000,
                eci.Position().z * 1000);
}

std::vector<double> Geodetic::ObtainLog() const {
  return {latitude_rad_, longitude_rad_};
}

double Geodetic::CalculateLatitudeRad(const Vector& _position_m_rad,
                                      const SimulationClock& _simulation_clock,
                                      const uint64_t& _offset_s) const {
  sgp4::Vector sgp4_pos(_position_m_rad.x_m() / 1000.0,
                        _position_m_rad.y_m() / 1000.0,
                        _position_m_rad.z_m() / 1000.0);
  sgp4::Eci sgp4_eci(_simulation_clock.date_time().AddSeconds(_offset_s),
                     sgp4_pos);
  sgp4::CoordGeodetic sgp4_geodetic = sgp4_eci.ToGeodetic();
  return sgp4_geodetic.latitude;
}

double Geodetic::CalculateLongitudeRad(const Vector& _position_m_rad,
                                       const SimulationClock& _simulation_clock,
                                       const uint64_t& _offset_s) const {
  sgp4::Vector sgp4_pos(_position_m_rad.x_m() / 1000.0,
                        _position_m_rad.y_m() / 1000.0,
                        _position_m_rad.z_m() / 1000.0);
  sgp4::Eci sgp4_eci(_simulation_clock.date_time().AddSeconds(_offset_s),
                     sgp4_pos);
  sgp4::CoordGeodetic sgp4_geodetic = sgp4_eci.ToGeodetic();
  return sgp4_geodetic.longitude;
}

double Geodetic::CalculateAltitudeM(const Vector& _position_m_rad,
                                    const SimulationClock& _simulation_clock,
                                    const uint64_t& _offset_s) const {
  sgp4::Vector sgp4_pos(_position_m_rad.x_m() / 1000.0,
                        _position_m_rad.y_m() / 1000.0,
                        _position_m_rad.z_m() / 1000.0);
  sgp4::Eci sgp4_eci(_simulation_clock.date_time().AddSeconds(_offset_s),
                     sgp4_pos);
  sgp4::CoordGeodetic sgp4_geodetic = sgp4_eci.ToGeodetic();
  return sgp4_geodetic.altitude;
}

std::array<double, 3> Geodetic::Intersection(
    const Vector& _position_m_rad,
    const Vector& _direction,
    const SimulationClock& _simulation_clock,
    const uint64_t& _offset_s) {
  // Earth WGS84 Ellipsoid
  double a = earth::kSemiMajorAxisM;
  double b = a;
  double c = earth::kSemiMinorAxisM;

  // Scaled Position
  Vector scaled_position_m_rad(_position_m_rad.x_m()/a,
                               _position_m_rad.y_m()/b,
                               _position_m_rad.z_m()/c);

  // Unit ray in direction of _direction
  Vector ray = _direction.Unit();

  // Quadratic formula
  ray.CompleteCoordinates();
  double A = std::pow(ray.r_m(), 2);
  double B = 2 * scaled_position_m_rad.Dot(ray);
  scaled_position_m_rad.CompleteCoordinates();
  double C = std::pow(scaled_position_m_rad.r_m(), 2) - 1;

  // If all roots imaginary, ray and ellipsoid do not intersect -> LOS exists
  double discriminant = std::pow(B, 2) - 4*A*C;
  Vector intersection_;
  if (discriminant > 0) {
    // Find first and second intersection
    double t1 = (-B + std::sqrt(discriminant)) / (2 * A);
    double t2 = (-B - std::sqrt(discriminant)) / (2 * A);

    // If both solutions are negative, then nothing between rx and tx
    Vector intersection_1;
    Vector intersection_2;
    if (t1 > 0) {
      Vector scaled = scaled_position_m_rad + (ray * t1);
      intersection_1 = Vector(scaled.x_m()*a, scaled.y_m()*b, scaled.z_m()*c);
    }
    if (t2 > 0) {
      Vector scaled = scaled_position_m_rad + (ray * t2);
      intersection_2 = Vector(scaled.x_m()*a, scaled.y_m()*b, scaled.z_m()*c);
    }

    // Determine which intersection point is closest to the position
    Vector difference_1 = (_position_m_rad - intersection_1);
    Vector difference_2 = (_position_m_rad - intersection_2);
    difference_1.CompleteCoordinates();
    difference_2.CompleteCoordinates();
    if (difference_1.r_m() < difference_2.r_m()) {
      intersection_ = intersection_1;
    } else {
      intersection_ = intersection_2;
    }
  }
  Geodetic local(intersection_, _simulation_clock, _offset_s);
  return {local.latitude_rad(), local.longitude_rad(), local.altitude_m()};
}

std::string Geodetic::ToString() const {
  std::stringstream stream;
  stream << std::fixed
         << std::setprecision(2)
         << "["
         << latitude_rad_
         << ", "
         << longitude_rad_
         << "]";
  return stream.str();
}

}  // namespace collaborate
}  // namespace osse
