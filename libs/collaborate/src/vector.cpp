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

#include "collaborate/vector.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "collaborate/earth.h"
#include "collaborate/util.h"

namespace osse {
namespace collaborate {

Vector::Vector()
    : x_m_(0),
      y_m_(0),
      z_m_(0),
      r_m_(0),
      rho_m_(0),
      theta_rad_(0),
      phi_rad_(0) {
}

Vector::Vector(double _x_m, double _y_m, double _z_m)
    : x_m_(_x_m),
      y_m_(_y_m),
      z_m_(_z_m),
      r_m_(0),
      rho_m_(0),
      theta_rad_(0),
      phi_rad_(0) {
}

void Vector::CompleteCoordinates() {
  r_m_ = std::sqrt(x_m_*x_m_ + y_m_*y_m_ + z_m_*z_m_);
  rho_m_ = std::sqrt(x_m_*x_m_ + y_m_*y_m_);
  theta_rad_ = CalculateThetaRad();
  phi_rad_ = CalculatePhiRad();
}

double Vector::CalculateThetaRad() const {
  double theta_rad = std::atan2(rho_m_, z_m_);
  if (theta_rad < 0) {
    theta_rad *= -1;
  }
  return theta_rad;
}

double Vector::CalculatePhiRad() const {
  double phi_rad = std::atan2(y_m_, x_m_);
  if (phi_rad < 0) {
    phi_rad += util::kTwoPiRad;
  }
  return phi_rad;
}

std::vector<double> Vector::ObtainLog() const {
  std::vector<double> log {x_m_, y_m_, z_m_};
  return log;
}

Vector Vector::Unit() const {
  double r_m = std::sqrt(x_m_*x_m_ + y_m_*y_m_ + z_m_*z_m_);
  return Vector(x_m_/r_m, y_m_/r_m, z_m_/r_m);
}

double Vector::Dot(const Vector& _other) const {
  return (x_m_*(_other.x_m()) + y_m_*(_other.y_m()) + z_m_*(_other.z_m()));
}

Vector Vector::Cross(const Vector& _other) const {
  return Vector((y_m_*_other.z_m() - z_m_*_other.y_m()),
                (z_m_*_other.x_m() - x_m_*_other.z_m()),
                (x_m_*_other.y_m() - y_m_*_other.x_m()));
}

double Vector::AngleBetween(const Vector& _other) const {
  double r_m = std::sqrt(x_m_*x_m_ + y_m_*y_m_ + z_m_*z_m_);
  return std::acos(Dot(_other) / (r_m*_other.r_m()));
}

Vector Vector::OrthoNormal(const Vector& _other) const {
  //  return ((*this - (_other * Dot(_other))).Unit());
  return ((_other - (*this * Dot(_other))).Unit());
}

Vector Vector::ConstraintToPlane(const Vector& _other,
                                 const Vector& _reference) const {
  //  std::cout << "reference: " << _reference << std::endl;
  //  Vector tempa = Unit();
  //  std::cout << "tempa: " << tempa << std::endl;
  //  Vector tempb = _other;
  //  std::cout << "tempb: " << tempb << std::endl;
  //  tempb = tempa.OrthoNormal(tempb);
  //  std::cout << "tempb: " << tempb << std::endl;
  //  double c1 = tempa.Dot(_reference);
  //  std::cout << "c1: " << c1 << std::endl;
  //  double c2 = tempb.Dot(_reference);
  //  std::cout << "c2: " << c2 << std::endl;
  //  std::cout << "reference: " << _reference << std::endl;
  //  exit(0);
  Vector first = Unit() * Unit().Dot(_reference);
  //  std::cout << "first: " << first << std::endl;
  Vector temp = Unit().OrthoNormal(_other);
  //  std::cout << "Unit().OrthoNormal(_other): " << temp << std::endl;
  //  std::cout << "c2: " << temp.Dot(_reference) << std::endl;
  Vector second = temp * (temp.Dot(_reference));
  //  std::cout << "second: " << second << std::endl;
  //  std::cout << "c: " << first + second << std::endl;
  //  exit(0);
  return first + second;
}

std::string Vector::ToString() const {
  std::stringstream stream;
  stream << std::fixed
         << std::setprecision(0)
         << "["
         << x_m_
         << ", "
         << y_m_
         << ", "
         << z_m_
         << "]";
  return stream.str();
}

}  // namespace collaborate
}  // namespace osse
