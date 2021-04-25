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

#include "collaborate/attitude_matrix.h"

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>

#include "collaborate/util.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

AttitudeMatrix::AttitudeMatrix(double _r0c0,
                               double _r0c1,
                               double _r0c2,
                               double _r1c0,
                               double _r1c1,
                               double _r1c2,
                               double _r2c0,
                               double _r2c1,
                               double _r2c2)
    : m_({{{{_r0c0, _r0c1, _r0c2}},
           {{_r1c0, _r1c1, _r1c2}},
           {{_r2c0, _r2c1, _r2c2}}}}),
      i_(InverseOfAttitudeMatrix()) {
}

AttitudeMatrix::AttitudeMatrix(const Vector& _x_axis,
                               const Vector& _y_axis,
                               const Vector& _z_axis)
    : m_(AttitudeMatrixFromAxes(_x_axis, _y_axis, _z_axis)),
      i_(InverseOfAttitudeMatrix()) {
}

AttitudeMatrix::AttitudeMatrix(const double& _roll_rad,
                               const double& _pitch_rad,
                               const double& _yaw_rad)
    : m_(AttitudeMatrixFromAngles(_roll_rad, _pitch_rad, _yaw_rad)),
      i_(InverseOfAttitudeMatrix()) {
}

std::string AttitudeMatrix::ToString() const {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(3) << "np.attitude_matrix(["
         << "[ " << m_[0][0] << ", " << m_[0][1] << ", " << m_[0][2] << "], "
         << "[ " << m_[1][0] << ", " << m_[1][1] << ", " << m_[1][2] << "], "
         << "[ " << m_[2][0] << ", " << m_[2][1] << ", " << m_[2][2] << "]"
         << "])" << std::endl;
  return stream.str();
}

AttitudeMatrix::Array AttitudeMatrix::AttitudeMatrixFromAxes(
    const Vector& _x_axis,
    const Vector& _y_axis,
    const Vector& _z_axis) const {
  Array m;
  double roll_rad_ = std::fmod(std::asin(-1 * _y_axis.z_m()),
                               util::kPiRad);
  double pitch_rad_ = std::fmod(std::atan2(_x_axis.z_m(), _z_axis.z_m()),
                                util::kTwoPiRad);
  double yaw_rad_ = std::fmod(std::atan2(_y_axis.x_m(), _y_axis.y_m()),
                              util::kTwoPiRad);
  m = AttitudeMatrixFromAngles(roll_rad_, pitch_rad_, yaw_rad_);
  return m;
}

AttitudeMatrix::Array AttitudeMatrix::AttitudeMatrixFromAngles(
    const double& _roll_rad,
    const double& _pitch_rad,
    const double& _yaw_rad) const {
  Array m;
  double sr = std::sin(_roll_rad);
  double cr = std::cos(_roll_rad);
  double sp = std::sin(_pitch_rad);
  double cp = std::cos(_pitch_rad);
  double sy = std::sin(_yaw_rad);
  double cy = std::cos(_yaw_rad);
  m = {{{{(cy*cp)+(sy*sr*sp), (sy*cr), (-1*cy*sp)+(sy*sr*cp)}},
        {{(-1*sy*cp)+(cy*sr*sp), (cy*cr), (sy*sp)+(cy*sr*cp)}},
        {{(cr*sp), (-1*sr), (cr * cp)}}}};
  for (int i = 0; i < kColumns; ++i) {
    for (int j = 0; j < kColumns; ++j) {
      if (std::abs(m[i][j]) <= std::numeric_limits<double>::epsilon()) {
        m[i][j] = 0;
      }
    }
  }
  return m;
}

AttitudeMatrix::Array AttitudeMatrix::InverseOfAttitudeMatrix() const {
  Array i;
  double det = AttitudeMatrixDeterminant();
  if (det != 0) {
    i = {{{{((m_[1][1] * m_[2][2]) - (m_[1][2] * m_[2][1])) / det,
            ((m_[0][2] * m_[2][1]) - (m_[0][1] * m_[2][2])) / det,
            ((m_[0][1] * m_[1][2]) - (m_[0][2] * m_[1][1])) / det}},
          {{((m_[1][2] * m_[2][0]) - (m_[1][0] * m_[2][2])) / det,
            ((m_[0][0] * m_[2][2]) - (m_[0][2] * m_[2][0])) / det,
            ((m_[0][2] * m_[1][0]) - (m_[0][0] * m_[1][2])) / det}},
          {{((m_[1][0] * m_[2][1]) - (m_[1][1] * m_[2][0])) / det,
            ((m_[0][1] * m_[2][0]) - (m_[0][0] * m_[2][1])) / det,
            ((m_[0][0] * m_[1][1]) - (m_[0][1] * m_[1][0])) / det}}}};
  } else {
    i = {{{{0, 0, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}};
  }
  return i;
}

double AttitudeMatrix::AttitudeMatrixDeterminant() const {
  return ((m_[0][0] * ((m_[1][1]*m_[2][2]) - (m_[1][2]*m_[2][1])))
          - (m_[0][1] * ((m_[1][0]*m_[2][2]) - (m_[2][0]*m_[1][2])))
          + (m_[0][2] * ((m_[1][0]*m_[2][1]) - (m_[1][1]*m_[2][0]))));
}

}  // namespace collaborate
}  // namespace osse
