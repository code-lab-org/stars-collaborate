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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_ATTITUDE_MATRIX_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_ATTITUDE_MATRIX_H_

#include <array>
#include <string>

#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

/// @class AttitudeMatrix
/// @brief A 2D (3x3) matrix for reference frame and attitude transformations
/// @details
/// \f[
/// {\bf{M}} =
/// \begin{bmatrix}
/// m_{0,0} & m_{0,1} & m_{0,2}
/// \\ m_{1,0} & m_{1,1} & m_{1,2}
/// \\ m_{2,0} & m_{2,1} & m_{2,2}
/// \end{bmatrix}
/// \f]
/// \f[ \bf{I} = \bf{M}^{-1} \f]
class AttitudeMatrix {
 public:
  /// @brief The number of columns in an Array
  static constexpr uint8_t kColumns = 3;
  /// @brief A row if an Array
  typedef std::array<double, kColumns> Row;
  /// @brief Underlying (wrapped) array of matrix
  typedef std::array<Row, kColumns> Array;
  /// @brief Constructor from explicit values
  /// @param[in] _r0c0 Row 0, Column 0
  /// @param[in] _r0c1 Row 0, Column 1
  /// @param[in] _r0c2 Row 0, Column 2
  /// @param[in] _r1c0 Row 1, Column 0
  /// @param[in] _r1c1 Row 1, Column 1
  /// @param[in] _r1c2 Row 1, Column 2
  /// @param[in] _r2c0 Row 2, Column 0
  /// @param[in] _r2c1 Row 2, Column 1
  /// @param[in] _r2c2 Row 2, Column 2
  /// @details
  /// \f[
  /// {\bf{M}} =
  /// \begin{bmatrix}
  /// r_0c_0 & r_0c_1 & r_0c_2
  /// \\ r_1c_0 & r_1c_1 & r_1c_2
  /// \\ r_2c_0 & r_2c_1 & r_2c_2
  /// \end{bmatrix}
  /// \f]
  AttitudeMatrix(double _r0c0,
                 double _r0c1,
                 double _r0c2,
                 double _r1c0,
                 double _r1c1,
                 double _r1c2,
                 double _r2c0,
                 double _r2c1,
                 double _r2c2);
  /// @brief Constructor From Axes
  /// @param[in] _x_axis X axis
  /// @param[in] _y_axis Y axis
  /// @param[in] _z_axis Z axis
  /// @details
  /// \f[ X = Roll~Axis \f]
  /// \f[ Y = Pitch~Axis \f]
  /// \f[ Z = Yaw~Axis \f]
  /// \f[ \psi = Roll = \sin^{-1}{-Y_Z}\ (\mathrm{mod}\ \pi) ~~~ (radians) \f]
  /// \f[ \theta = Pitch
  ///            = \tan^{-1}{\frac{X_Z}{Z_Z}}\ (\mathrm{mod}\ 2\pi)~~~(radians)
  /// \f]
  /// \f[ \phi = Yaw
  ///          = \tan^{-1}{\frac{Y_X}{Y_Y}}\ (\mathrm{mod}\ 2\pi) ~~~ (radians)
  /// \f]
  /// \f[
  /// {\bf{M}} =
  /// \begin{bmatrix}
  /// \cos{\psi}\cos{\theta}+\sin{\psi}\sin{\phi}\sin{\theta} &
  /// \sin{\psi}\cos{\phi} &
  /// -\cos{\psi}\sin{\theta}+\sin{\psi}\sin{\phi}\cos{\theta}
  /// \\ -\sin{\psi}\cos{\theta}+\cos{\psi}\sin{\phi}\sin{\theta} &
  /// \cos{\psi}\cos{\phi} &
  /// \sin{\psi}\sin{\theta}+\cos{\psi}\sin{\phi}\cos{\theta}
  /// \\ \cos{\psi}\sin{\phi} &
  /// -\sin{\phi} &
  /// \cos{\phi}\cos{\theta}
  /// \end{bmatrix}
  /// \f]
  AttitudeMatrix(const Vector& _x_axis,
                 const Vector& _y_axis,
                 const Vector& _z_axis);
  /// @brief Constructor From Angles
  /// @param[in] _roll_rad Roll angle about the x axis (radians)
  /// @param[in] _pitch_rad Pitch angle about the x axis (radians)
  /// @param[in] _yaw_rad Yaw angle about the x axis (radians)
  /// @details
  /// \f[ \psi = Roll ~~~ (radians) \f]
  /// \f[ \theta = Pitch ~~~ (radians) \f]
  /// \f[ \phi = Yaw ~~~ (radians) \f]
  /// \f[
  /// {\bf{M}} =
  /// \begin{bmatrix}
  /// \cos{\psi}\cos{\theta}+\sin{\psi}\sin{\phi}\sin{\theta} &
  /// \sin{\psi}\cos{\phi} &
  /// -\cos{\psi}\sin{\theta}+\sin{\psi}\sin{\phi}\cos{\theta}
  /// \\ -\sin{\psi}\cos{\theta}+\cos{\psi}\sin{\phi}\sin{\theta} &
  /// \cos{\psi}\cos{\phi} &
  /// \sin{\psi}\sin{\theta}+\cos{\psi}\sin{\phi}\cos{\theta}
  /// \\ \cos{\psi}\sin{\phi} &
  /// -\sin{\phi} &
  /// \cos{\phi}\cos{\theta}
  /// \end{bmatrix}
  /// \f]
  AttitudeMatrix(const double& _roll_rad,
                 const double& _pitch_rad,
                 const double& _yaw_rad);
  /// @brief Transforms a vector to a new coordinate system
  /// @param[in] _vector Vector
  /// @returns Transformed vector
  /// @details
  /// \f[
  /// {\bf M}\vec{v} =
  /// \begin{bmatrix}
  /// M_{0,0} & M_{0,1} & M_{0,2}
  /// \\ M_{1,0} & M_{1,1} & M_{1,2}
  /// \\ M_{2,0} & M_{2,1} & M_{2,2}
  /// \end{bmatrix}
  /// \begin{bmatrix}
  /// v_{0,0} \\ v_{1,0} \\ v_{2,0}
  /// \end{bmatrix}
  /// = \vec{t} =
  /// \begin{bmatrix}
  /// t_{0,0} \\ t_{1,0} \\ t_{2,0}
  /// \end{bmatrix}
  /// \f]
  Vector TransformVector(const Vector& _vector) const {
    return Vector(((m_[0][0] * _vector.x_m())
                   + (m_[0][1] * _vector.y_m())
                   + (m_[0][2] * _vector.z_m())),
                  ((m_[1][0] * _vector.x_m())
                   + (m_[1][1] * _vector.y_m())
                   + (m_[1][2] * _vector.z_m())),
                  ((m_[2][0] * _vector.x_m())
                   + (m_[2][1] * _vector.y_m())
                   + (m_[2][2] * _vector.z_m())));
  }
  /// @brief Performs an inverse transformation on a vector
  /// @param[in] _vector Vector
  /// @returns Inverted vector
  /// @details
  /// \f[
  /// {\bf{I}}\vec{v} =
  /// {\bf{M}}^{-1}
  /// \begin{bmatrix}
  /// v_{0,0} \\ v_{1,0} \\ v_{2,0}
  /// \end{bmatrix}
  /// = \vec{i} =
  /// \begin{bmatrix}
  /// i_{0,0} \\ i_{1,0} \\ i_{2,0}
  /// \end{bmatrix}
  /// \f]
  Vector InvertVector(const Vector& _vector) const {
    return Vector(((i_[0][0] * _vector.x_m())
                   + (i_[0][1] * _vector.y_m())
                   + (i_[0][2] * _vector.z_m())),
                  ((i_[1][0] * _vector.x_m())
                   + (i_[1][1] * _vector.y_m())
                   + (i_[1][2] * _vector.z_m())),
                  ((i_[2][0] * _vector.x_m())
                   + (i_[2][1] * _vector.y_m())
                   + (i_[2][2] * _vector.z_m())));
  }
  /// @brief Outputs the matrix to a string
  /// @returns AttitudeMatrix as a string
  std::string ToString() const;

 private:
  /// @brief Overloads the [] operator to access a row
  /// @param[in] _column The number of the column
  /// @returns Row
  Row operator[] (int _column) const {return m_[_column];}
  /// @brief Calculate From Axes
  /// @param[in] _x_axis X axis
  /// @param[in] _y_axis Y axis
  /// @param[in] _z_axis Z axis
  /// @returns An attitude matrix calculated from axes
  /// @details
  /// \f[ X = Roll~Axis \f]
  /// \f[ Y = Pitch~Axis \f]
  /// \f[ Z = Yaw~Axis \f]
  /// \f[ \psi = Roll = \sin^{-1}{-Y_Z}\ (\mathrm{mod}\ \pi) ~~~ (radians) \f]
  /// \f[ \theta = Pitch
  ///            = \tan^{-1}{\frac{X_Z}{Z_Z}}\ (\mathrm{mod}\ 2\pi)~~~(radians)
  /// \f]
  /// \f[ \phi = Yaw
  ///          = \tan^{-1}{\frac{Y_X}{Y_Y}}\ (\mathrm{mod}\ 2\pi) ~~~ (radians)
  /// \f]
  /// \f[
  /// {\bf{M}} =
  /// \begin{bmatrix}
  /// \cos{\psi}\cos{\theta}+\sin{\psi}\sin{\phi}\sin{\theta} &
  /// \sin{\psi}\cos{\phi} &
  /// -\cos{\psi}\sin{\theta}+\sin{\psi}\sin{\phi}\cos{\theta}
  /// \\ -\sin{\psi}\cos{\theta}+\cos{\psi}\sin{\phi}\sin{\theta} &
  /// \cos{\psi}\cos{\phi} &
  /// \sin{\psi}\sin{\theta}+\cos{\psi}\sin{\phi}\cos{\theta}
  /// \\ \cos{\psi}\sin{\phi} &
  /// -\sin{\phi} &
  /// \cos{\phi}\cos{\theta}
  /// \end{bmatrix}
  /// \f]
  Array AttitudeMatrixFromAxes(const Vector& _x_axis,
                               const Vector& _y_axis,
                               const Vector& _z_axis) const;
  /// @brief Calculate From Angles
  /// @param[in] _roll_rad Roll angle about the x axis (radians)
  /// @param[in] _pitch_rad Pitch angle about the x axis (radians)
  /// @param[in] _yaw_rad Yaw angle about the x axis (radians)
  /// @returns An attitude matrix calculated from angles
  /// @details
  /// \f[ \psi = Roll ~~~ (radians) \f]
  /// \f[ \theta = Pitch ~~~ (radians) \f]
  /// \f[ \phi = Yaw ~~~ (radians) \f]
  /// \f[
  /// {\bf{M}} =
  /// \begin{bmatrix}
  /// \cos{\psi}\cos{\theta}+\sin{\psi}\sin{\phi}\sin{\theta} &
  /// \sin{\psi}\cos{\phi} &
  /// -\cos{\psi}\sin{\theta}+\sin{\psi}\sin{\phi}\cos{\theta}
  /// \\ -\sin{\psi}\cos{\theta}+\cos{\psi}\sin{\phi}\sin{\theta} &
  /// \cos{\psi}\cos{\phi} &
  /// \sin{\psi}\sin{\theta}+\cos{\psi}\sin{\phi}\cos{\theta}
  /// \\ \cos{\psi}\sin{\phi} &
  /// -\sin{\phi} &
  /// \cos{\phi}\cos{\theta}
  /// \end{bmatrix}
  /// \f]
  Array AttitudeMatrixFromAngles(const double& _roll_rad,
                                 const double& _pitch_rad,
                                 const double& _yaw_rad) const;
  /// @brief Calculate the inverse matrix
  /// @returns Inverse matrix
  /// @details
  /// \f[
  /// {\bf{I}} =
  /// \left(\frac{1}{\det{{\bf{M}}}}\right)
  /// \begin{bmatrix}
  /// m_{1,1} m_{2,2} - m_{1,2} m_{2,1} &
  /// m_{0,2} m_{2,1} - m_{0,1} m_{2,2} &
  /// m_{0,1} m_{1,2} - m_{0,2} m_{1,1}
  /// \\ m_{1,2} m_{2,0} - m_{1,0} m_{2,2} &
  /// m_{0,0} m_{2,2} - m_{0,2} m_{2,0} &
  /// m_{0,2} m_{1,0} - m_{0,0} m_{1,2}
  /// \\ m_{1,0} m_{2,1} - m_{1,1} m_{2,0} &
  /// m_{0,1} m_{2,0} - m_{0,0} m_{2,1} &
  /// m_{0,0} m_{1,1} - m_{0,1} m_{1,0}
  /// \end{bmatrix}
  /// \f]
  Array InverseOfAttitudeMatrix() const;
  /// @brief Calculate the determinant of the matrix
  /// @returns Determinant
  /// @details
  /// \f[ \det{{\bf{M}}} = \f]
  /// \f[ (m_{0,0} (m_{1,1}m_{2,2} - m_{1,2}m_{2,1})) \f]
  /// \f[ -(m_{0,1} (m_{1,0}m_{2,2} - m_{2,0}m_{1,2})) \f]
  /// \f[ +(m_{0,2} (m_{1,0}m_{2,1} - m_{1,1}m_{2,0})) \f]
  double AttitudeMatrixDeterminant() const;
  /// @brief AttitudeMatrix
  Array m_;
  /// @brief Inverse matrix
  Array i_;
};

/// @brief Output vector to a stream
/// @param[in,out] strm stream to output to
/// @param[in] _attitude_matrix Matrix
inline std::ostream& operator<<(std::ostream& strm,
                                const AttitudeMatrix& _attitude_matrix) {
  return strm << _attitude_matrix.ToString();
}

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_ATTITUDE_MATRIX_H_
