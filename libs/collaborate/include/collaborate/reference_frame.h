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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_REFERENCE_FRAME_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_REFERENCE_FRAME_H_

#include <ostream>
#include <string>
#include <vector>

#include "collaborate/attitude_matrix.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

/// @class ReferenceFrame
/// @brief An attitude reference frame
/// @image html raster/frames.png width=350
/// @image latex vector/frames.pdf width=0.5\textwidth
/// @details
/// \f[
/// \vec{x}_{axis}, \vec{y}_{axis}, \vec{z}_{axis} =
/// \begin{bmatrix} x_x\\x_y\\x_z \end{bmatrix},~
/// \begin{bmatrix} y_x\\y_y\\y_z \end{bmatrix},~
/// \begin{bmatrix} z_x\\z_y\\z_z \end{bmatrix} ~~~(unit~vectors)
/// \f]
/// \f[
/// {\bf{A}} = Attitude~AttitudeMatrix =
/// \begin{bmatrix}
/// a_{0,0} & a_{0,1} & a_{0,2}
/// \\ a_{1,0} & a_{1,1} & a_{1,2}
/// \\ a_{2,0} & a_{2,1} & a_{2,2}
/// \end{bmatrix}
/// \f]
class ReferenceFrame {
 public:
  /// @brief Constructor from axes
  /// @param[in] _x_axis X-axis (unit)
  /// @param[in] _y_axis Y-axis (unit)
  /// @param[in] _z_axis Z-axis (unit)
  ReferenceFrame(const Vector& _x_axis,
                 const Vector& _y_axis,
                 const Vector& _z_axis);
  /// @brief Constructor from angles
  /// @param[in] _roll_rad Rotation about the x-axis
  /// @param[in] _pitch_rad Rotation about the y-axis
  /// @param[in] _yaw_rad Rotation about the z-axis
  ReferenceFrame(const double& _roll_rad,
                 const double& _pitch_rad,
                 const double& _yaw_rad);
  /// @brief Constructor from reference frame and rotation angles
  /// @param[in] _frame Reference frame
  /// @param[in] _roll_rad Rotation about the x-axis
  /// @param[in] _pitch_rad Rotation about the y-axis
  /// @param[in] _yaw_rad Rotation about the z-axis
  ReferenceFrame(const ReferenceFrame& _frame,
                 const double& _roll_rad,
                 const double& _pitch_rad,
                 const double& _yaw_rad);
  /// @brief Constructor from two reference frames and rotation angles
  /// @param[in] _frame_1 First reference frame
  /// @param[in] _frame_2 Second reference frame
  /// @param[in] _roll_rad Rotation about the x-axis
  /// @param[in] _pitch_rad Rotation about the y-axis
  /// @param[in] _yaw_rad Rotation about the z-axis
  ReferenceFrame(const ReferenceFrame& _frame_1,
                 const ReferenceFrame& _frame_2,
                 const double& _roll_rad,
                 const double& _pitch_rad,
                 const double& _yaw_rad);
  /// @brief Updates axes relative to a single reference frame
  /// @param[in] _frame Reference frame
  /// @details
  /// \f[ {\bf{M}}_{this} = This~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F} = Reference~ReferenceFrame~Attitude~AttitudeMatrix \f]
  /// \f[
  /// \vec{x}_{earth} = \begin{bmatrix} 1 \\ 0 \\ 0 \end{bmatrix},~~~
  /// \vec{y}_{earth} = \begin{bmatrix} 0 \\ 1 \\ 0 \end{bmatrix},~~~
  /// \vec{z}_{earth} = \begin{bmatrix} 0 \\ 0 \\ 1 \end{bmatrix}
  /// \f]
  /// \f[
  /// \vec{x} =
  /// \left(\left(\vec{x}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F}\right)
  /// \f]
  /// \f[
  /// \vec{y} =
  /// \left(\left(\vec{y}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F}\right)
  /// \f]
  /// \f[
  /// \vec{y} =
  /// \left(\left(\vec{y}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F}\right)
  /// \f]
  void Update(const ReferenceFrame& _frame);
  /// @brief Updates axes relative to a single reference frame
  /// @param[in] _frame_1 First reference frame
  /// @param[in] _frame_2 Second reference frame
  /// @details
  /// \f[ {\bf{M}}_{this} = This~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F1} = Reference~ReferenceFrame~1~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F2} = Reference~ReferenceFrame~2~Attitude~AttitudeMatrix \f]
  /// \f[
  /// \vec{x}_{earth} = \begin{bmatrix} 1 \\ 0 \\ 0 \end{bmatrix},~~~
  /// \vec{y}_{earth} = \begin{bmatrix} 0 \\ 1 \\ 0 \end{bmatrix},~~~
  /// \vec{z}_{earth} = \begin{bmatrix} 0 \\ 0 \\ 1 \end{bmatrix}
  /// \f]
  /// \f[
  /// \vec{x} =
  /// \left(
  /// \left(\left(\vec{x}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F2}\right)
  /// {\bf{M}}_{F1}\right)
  /// \f]
  /// \f[
  /// \vec{y} =
  /// \left(
  /// \left(\left(\vec{y}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F2}\right)
  /// {\bf{M}}_{F1}\right)
  /// \f]
  /// \f[
  /// \vec{z} =
  /// \left(
  /// \left(\left(\vec{z}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F2}\right)
  /// {\bf{M}}_{F1}\right)
  /// \f]
  void Update(const ReferenceFrame& _frame_1,
              const ReferenceFrame& _frame_2);
  /// @brief Logs the ReferenceFrame
  /// @returns A vector of doubles
  std::vector<double> ObtainLog() const;
  /// @brief Set z-axis
  /// @param[in] _z_axis Z-axis
  void set_z_axis(const Vector& _z_axis) {z_axis_ = _z_axis;}
  /// @brief Set y-axis
  /// @param[in] _y_axis Y-axis
  void set_y_axis(const Vector& _y_axis) {y_axis_ = _y_axis;}
  /// @brief Set x-axis
  /// @param[in] _x_axis X-axis
  void set_x_axis(const Vector& _x_axis) {x_axis_ = _x_axis;}
  /// @brief Get attitude attitude_matrix
  /// @returns attitude_ Attitude attitude_matrix
  const AttitudeMatrix& attitude() const {return attitude_;}
  /// @brief Get z-axis
  /// @returns z_axis_ Z-axis
  const Vector& z_axis() const {return z_axis_;}
  /// @brief Get y-axis
  /// @returns y_axis_ Y-axis
  const Vector& y_axis() const {return y_axis_;}
  /// @brief Get x-axis
  /// @returns x_axis_ X-axis
  const Vector& x_axis() const {return x_axis_;}
  /// @brief Outputs ReferenceFrame to a string
  /// @returns ReferenceFrame as a string
  std::string ToString() const;

 private:
  /// @brief Transforms Earth's x-axis through single reference
  /// @param[in] _frame Reference frame
  /// @returns Earth's transformed x-axis
  /// @details
  /// \f[ {\bf{M}}_{this} = This~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F} = Reference~ReferenceFrame~Attitude~AttitudeMatrix \f]
  /// \f[ \vec{x}_{earth} = \begin{bmatrix} 1 \\ 0 \\ 0 \end{bmatrix} \f]
  /// \f[
  /// \vec{x} =
  /// \left(\left(\vec{x}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F}\right)
  /// \f]
  Vector TransformXAxis(const ReferenceFrame& _frame) const;
  /// @brief Transforms Earth's y-axis through single reference
  /// @param[in] _frame Reference frame
  /// @returns Earth's transformed y-axis
  /// @details
  /// \f[ {\bf{M}}_{this} = This~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F} = Reference~ReferenceFrame~Attitude~AttitudeMatrix \f]
  /// \f[ \vec{y}_{earth} = \begin{bmatrix} 0 \\ 1 \\ 0 \end{bmatrix} \f]
  /// \f[
  /// \vec{y} =
  /// \left(\left(\vec{y}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F}\right)
  /// \f]
  Vector TransformYAxis(const ReferenceFrame& _frame) const;
  /// @brief Transforms Earth's z-axis through single reference
  /// @param[in] _frame Reference frame
  /// @returns Earth's transformed z-axis
  /// @details
  /// \f[ {\bf{M}}_{this} = This~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F} = Reference~ReferenceFrame~Attitude~AttitudeMatrix \f]
  /// \f[ \vec{z}_{earth} = \begin{bmatrix} 0 \\ 0 \\ 1 \end{bmatrix} \f]
  /// \f[
  /// \vec{z} =
  /// \left(\left(\vec{z}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F}\right)
  /// \f]
  Vector TransformZAxis(const ReferenceFrame& _frame) const;
  /// @brief Transforms Earth's x-axis through two reference frames
  /// @param[in] _frame_1 First reference frame
  /// @param[in] _frame_2 Second reference frame
  /// @returns Earth's transformed x-axis
  /// @details
  /// \f[ {\bf{M}}_{this} = This~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F1} = Reference~ReferenceFrame~1~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F2} = Reference~ReferenceFrame~2~Attitude~AttitudeMatrix \f]
  /// \f[ \vec{x}_{earth} = \begin{bmatrix} 1 \\ 0 \\ 0 \end{bmatrix} \f]
  /// \f[
  /// \vec{x} =
  /// \left(
  /// \left(\left(\vec{x}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F2}\right)
  /// {\bf{M}}_{F1}\right)
  /// \f]
  Vector TransformXAxis(const ReferenceFrame& _frame_1,
                        const ReferenceFrame& _frame_2) const;
  /// @brief Transforms Earth's y-axis through two reference frames
  /// @param[in] _frame_1 First reference frame
  /// @param[in] _frame_2 Second reference frame
  /// @returns Earth's transformed y-axis
  /// @details
  /// \f[ {\bf{M}}_{this} = This~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F1} = Reference~ReferenceFrame~1~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F2} = Reference~ReferenceFrame~2~Attitude~AttitudeMatrix \f]
  /// \f[ \vec{y}_{earth} = \begin{bmatrix} 0 \\ 1 \\ 0 \end{bmatrix} \f]
  /// \f[
  /// \vec{y} =
  /// \left(
  /// \left(\left(\vec{y}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F2}\right)
  /// {\bf{M}}_{F1}\right)
  /// \f]
  Vector TransformYAxis(const ReferenceFrame& _frame_1,
                        const ReferenceFrame& _frame_2) const;
  /// @brief Transforms Earth's z-axis through two reference frames
  /// @param[in] _frame_1 First reference frame
  /// @param[in] _frame_2 Second reference frame
  /// @returns Earth's transformed z-axis
  /// @details
  /// \f[ {\bf{M}}_{this} = This~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F1} = Reference~ReferenceFrame~1~Attitude~AttitudeMatrix \f]
  /// \f[ {\bf{M}}_{F2} = Reference~ReferenceFrame~2~Attitude~AttitudeMatrix \f]
  /// \f[ \vec{z}_{earth} = \begin{bmatrix} 0 \\ 0 \\ 1 \end{bmatrix} \f]
  /// \f[
  /// \vec{z} =
  /// \left(
  /// \left(\left(\vec{z}_{earth}{\bf{M}}_{this}\right){\bf{M}}_{F2}\right)
  /// {\bf{M}}_{F1}\right)
  /// \f]
  Vector TransformZAxis(const ReferenceFrame& _frame_1,
                        const ReferenceFrame& _frame_2) const;
  /// @brief Attitude attitude_matrix
  AttitudeMatrix attitude_;
  /// @brief X-axis
  Vector x_axis_;
  /// @brief Y-axis
  Vector y_axis_;
  /// @brief Z-axis
  Vector z_axis_;
};

/// @brief Output frame to a stream
/// @param[in,out] strm stream to output to
/// @param[in] _frame ReferenceFrame
inline std::ostream& operator<<(std::ostream& strm,
                                const ReferenceFrame& _frame) {
  return strm << _frame.ToString();
}

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_REFERENCE_FRAME_H_
