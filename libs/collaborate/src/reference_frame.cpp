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

#include "collaborate/reference_frame.h"

#include <sstream>
#include <string>
#include <vector>

#include "collaborate/attitude_matrix.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

ReferenceFrame::ReferenceFrame(const Vector& _x_axis,
                               const Vector& _y_axis,
                               const Vector& _z_axis)
    : attitude_(AttitudeMatrix(_x_axis, _y_axis, _z_axis)),
      x_axis_(_x_axis),
      y_axis_(_y_axis),
      z_axis_(_z_axis) {
}

ReferenceFrame::ReferenceFrame(const double& _roll_rad,
                               const double& _pitch_rad,
                               const double& _yaw_rad)
    : attitude_(AttitudeMatrix(_roll_rad, _pitch_rad, _yaw_rad)),
      x_axis_(Vector(1, 0, 0)),
      y_axis_(Vector(0, 1, 0)),
      z_axis_(Vector(0, 0, 1)) {
}

ReferenceFrame::ReferenceFrame(const ReferenceFrame& _other,
                               const double& _roll_rad,
                               const double& _pitch_rad,
                               const double& _yaw_rad)
    : attitude_(AttitudeMatrix(_roll_rad, _pitch_rad, _yaw_rad)),
      x_axis_(TransformXAxis(_other)),
      y_axis_(TransformYAxis(_other)),
      z_axis_(TransformZAxis(_other)) {
}

ReferenceFrame::ReferenceFrame(const ReferenceFrame& _other_1,
                               const ReferenceFrame& _other_2,
                               const double& _roll_rad,
                               const double& _pitch_rad,
                               const double& _yaw_rad)
    : attitude_(AttitudeMatrix(_roll_rad, _pitch_rad, _yaw_rad)),
      x_axis_(TransformXAxis(_other_1, _other_2)),
      y_axis_(TransformYAxis(_other_1, _other_2)),
      z_axis_(TransformZAxis(_other_1, _other_2)) {
}

Vector ReferenceFrame::TransformXAxis(const ReferenceFrame& _other) const {
  Vector x_axis = attitude_.TransformVector(Vector(1, 0, 0));
  x_axis = _other.attitude().TransformVector(x_axis);
  return x_axis;
}

Vector ReferenceFrame::TransformYAxis(const ReferenceFrame& _other) const {
  Vector y_axis = attitude_.TransformVector(Vector(0, 1, 0));
  y_axis = _other.attitude().TransformVector(y_axis);
  return y_axis;
}

Vector ReferenceFrame::TransformZAxis(const ReferenceFrame& _other) const {
  Vector z_axis = attitude_.TransformVector(Vector(0, 0, 1));
  z_axis = _other.attitude().TransformVector(z_axis);
  return z_axis;
}

Vector ReferenceFrame::TransformXAxis(const ReferenceFrame& _other_1,
                                      const ReferenceFrame& _other_2) const {
  Vector x_axis = attitude_.TransformVector(Vector(1, 0, 0));
  x_axis = _other_2.attitude().TransformVector(x_axis);
  x_axis = _other_1.attitude().TransformVector(x_axis);
  return x_axis;
}

Vector ReferenceFrame::TransformYAxis(const ReferenceFrame& _other_1,
                                      const ReferenceFrame& _other_2) const {
  Vector y_axis = attitude_.TransformVector(Vector(0, 1, 0));
  y_axis = _other_2.attitude().TransformVector(y_axis);
  y_axis = _other_1.attitude().TransformVector(y_axis);
  return y_axis;
}

Vector ReferenceFrame::TransformZAxis(const ReferenceFrame& _other_1,
                                      const ReferenceFrame& _other_2) const {
  Vector z_axis = attitude_.TransformVector(Vector(0, 0, 1));
  z_axis = _other_2.attitude().TransformVector(z_axis);
  z_axis = _other_1.attitude().TransformVector(z_axis);
  return z_axis;
}

void ReferenceFrame::Update(const ReferenceFrame& _other) {
  // X-Axis
  x_axis_ = attitude_.TransformVector(Vector(1, 0, 0));
  x_axis_ = _other.attitude().TransformVector(x_axis_);

  // Y-Axis
  y_axis_ = attitude_.TransformVector(Vector(0, 1, 0));
  y_axis_ = _other.attitude().TransformVector(y_axis_);

  // Z-Axis
  z_axis_ = attitude_.TransformVector(Vector(0, 0, 1));
  z_axis_ = _other.attitude().TransformVector(z_axis_);
}

void ReferenceFrame::Update(const ReferenceFrame& _other_1,
                            const ReferenceFrame& _other_2) {
  // X-Axis
  x_axis_ = attitude_.TransformVector(Vector(1, 0, 0));
  x_axis_ = _other_2.attitude().TransformVector(x_axis_);
  x_axis_ = _other_1.attitude().TransformVector(x_axis_);

  // Y-Axis
  y_axis_ = attitude_.TransformVector(Vector(0, 1, 0));
  y_axis_ = _other_2.attitude().TransformVector(y_axis_);
  y_axis_ = _other_1.attitude().TransformVector(y_axis_);

  // Z-Axis
  z_axis_ = attitude_.TransformVector(Vector(0, 0, 1));
  z_axis_ = _other_2.attitude().TransformVector(z_axis_);
  z_axis_ = _other_1.attitude().TransformVector(z_axis_);
}

std::vector<double> ReferenceFrame::ObtainLog() const {
  std::vector<double> log;
  std::vector<double> axis_log;
  for (auto axis : {x_axis_, y_axis_, z_axis_}) {
    axis_log = axis.ObtainLog();
    log.insert(log.end(), axis_log.begin(), axis_log.end());
  }
  return log;
}

std::string ReferenceFrame::ToString() const {
  std::stringstream stream;
  stream << "x_axis = " << x_axis_
         << "y_axis = " << y_axis_
         << "z_axis = " << z_axis_
         << "attitude = " << attitude_;
  return stream.str();
}

}  // namespace collaborate
}  // namespace osse
