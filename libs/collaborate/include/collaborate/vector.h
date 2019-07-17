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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_VECTOR_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_VECTOR_H_

#include <limits>
#include <string>
#include <vector>

namespace osse {
namespace collaborate {

/// @class Vector
/// @brief An element of the real coordinate space (cartesian and spherical)
/// @details
/// \f[ \vec{v} = \begin{bmatrix} v_x\\v_y\\v_z \end{bmatrix} ~~~(meters) \f]
class Vector {
 public:
  /// @brief Default Constructor
  /// @details
  /// \f[ \vec{s} = \begin{bmatrix} 0 \\ 0 \\ 0 \end{bmatrix} ~~~(meters) \f]
  Vector();
  /// @brief 3D Cartesian Constructor - Calculates the 4th (magnitude)
  /// @param[in] _x_m X value (meters)
  /// @param[in] _y_m Y value (meters)
  /// @param[in] _z_m Z value (meters)
  /// @details
  /// \f[ \vec{s} = \begin{bmatrix} s_x\\s_y\\s_z \end{bmatrix} ~~~(meters) \f]
  Vector(double _x_m, double _y_m, double _z_m);
  /// @brief Updates the vector's spherical and cylindrical coordinates
  /// @details
  /// \f[ r = \sqrt{x^2 + y^2 + z^2} ~~~(meters) \f]
  /// \f[ \rho = \sqrt{x^2 + y^2} ~~~(meters) \f]
  /// \f[
  /// \theta =
  /// \left\lvert\arctan{\left(\frac{\rho}{z}\right)}\right\rvert ~~~(radians)
  /// \f]
  /// \f[ \phi = \arctan{\frac{y}{x}} (\mathrm{mod}\ 2\pi) ~~~(radians) \f]
  void CompleteCoordinates();
  /// @brief Calculates a theta value between 0 and pi (radians)
  /// @returns A theta value between 0 and pi (radians)
  /// @details
  /// \f[
  /// \theta =
  /// \left\lvert\arctan{\left(\frac{\rho}{z}\right)}\right\rvert ~~~(radians)
  /// \f]
  double CalculateThetaRad() const;
  /// @brief Calculates a phi value between 0 and 2*pi (radians)
  /// @returns A phi value between 0 and 2*pi (radians)
  /// @details
  /// \f[
  /// \phi = \arctan{\left(\frac{y}{x}\right)} (\mathrm{mod}\ 2\pi) ~~~(radians)
  /// \f]
  double CalculatePhiRad() const;
  /// @brief Logs the Vector
  /// @returns A vector of doubles
  std::vector<double> ObtainLog() const;
  /// @brief Negative operator
  /// @returns Negative of the vector
  /// @details
  /// \f[ -\vec{s} = \begin{bmatrix} -x \\ -y \\ -z \end{bmatrix}~~~(meters) \f]
  Vector operator-() const {return Vector(-x_m_, -y_m_, -z_m_);}
  /// @brief Addition operator
  /// @param[in] _other Other vector
  /// @returns Sum of the vectors
  /// @details
  /// \f[
  /// \vec{s} + \vec{t} =
  /// \begin{bmatrix} s_x + t_x \\ s_y + t_y \\ s_z + t_z \end{bmatrix}
  /// ~~~(meters)
  /// \f]
  Vector operator+(const Vector& _other) const {
    return Vector(x_m_+_other.x_m(), y_m_+_other.y_m(), z_m_+_other.z_m());
  }
  /// @brief Subtraction operator
  /// @param[in] _other Other vector
  /// @returns Difference between the vectors
  /// @details
  /// \f[
  /// \vec{s} - \vec{t} =
  /// \begin{bmatrix} s_x - t_x \\ s_y - t_y \\ s_z - t_z \end{bmatrix}
  /// ~~~(meters)
  /// \f]
  Vector operator-(const Vector& _other) const {
    return Vector(x_m_-_other.x_m(), y_m_-_other.y_m(), z_m_-_other.z_m());
  }
  /// @brief Produces a unit vector in the current direction
  /// @returns Unit vector in the current direction
  /// @details
  /// \f[ \hat{s} = \frac{\vec{s}}{\lvert s\rvert} \f]
  Vector Unit() const;
  /// @brief Calculates the dot product
  /// @param[in] _other Other vector
  /// @returns Dot product
  /// @details
  /// \f[ \vec{s} \cdot \vec{t} = s_x t_x + s_y t_y + s_z t_z \f]
  double Dot(const Vector& _other) const;
  /// @brief Calculates the cross product between two vectors
  /// @param[in] _other Other vector
  /// @returns Cross product
  /// @details
  /// \f[
  /// \vec{s} \times \vec{t} =
  /// \begin{bmatrix}
  /// s_y t_z - s_z t_y \\ s_z t_x - s_x t_z \\ s_x t_y - s_y t_x
  /// \end{bmatrix}
  /// ~~~(meters)
  /// \f]
  Vector Cross(const Vector& _other) const;
  /// @brief Multiplication operator
  /// @param[in] _scalar Scaling value
  /// @returns Scaled vector
  /// @details
  /// \f[ c\vec{s}=\begin{bmatrix}c s_x\\c s_y\\c s_z\end{bmatrix}~~~(meters)
  /// \f]
  Vector operator*(const double& _scalar) const {
    return Vector((x_m_*_scalar), (y_m_*_scalar), (z_m_*_scalar));
  }
  /// @brief Division operator
  /// @param[in] _scalar Scaling value
  /// @returns Scaled vector
  /// @details
  /// \f[
  /// c\vec{s}=\begin{bmatrix}\frac{s_x}{c}\\\frac{s_y}{c}\\\frac{s_z}{c}
  /// \end{bmatrix}~~~(meters)
  /// \f]
  Vector operator/(const double& _scalar) const {
    return Vector((x_m_/_scalar), (y_m_/_scalar), (z_m_/_scalar));
  }
  /// @brief Boolean operator
  /// @returns True if vector is non-zero
  explicit operator bool() const {
    return (x_m_ >= std::numeric_limits<double>::epsilon()
            && y_m_ >= std::numeric_limits<double>::epsilon()
            && z_m_ >= std::numeric_limits<double>::epsilon());
  }
  /// @brief Calculate the angle between two coordinates
  /// @param[in] _other Other coordinates
  /// @returns Angle between the coordinates (radians)
  /// @details
  /// \f[
  /// \angle(s, t) =
  /// \arccos{\left(\frac{s \cdot t}{\lvert s\rvert\lvert t\rvert}\right)}
  /// \f]
  double AngleBetween(const Vector& _other) const;
  /// @brief Calculate the unit vector orthonormal to other vector
  /// @param[in] _other Other coordinates
  /// @returns Unit vector orthonormal to other vector
  Vector OrthoNormal(const Vector& _other) const;
  /// @brief Constrain to plane
  /// @param[in] _other Other vector
  /// @param[in] _reference Reference vector
  /// @returns Vector in the same plane as other and reference
  Vector ConstraintToPlane(const Vector& _other,
                           const Vector& _reference) const;
  /// @brief Outputs the vector to a string
  /// @returns Vector as a string
  std::string ToString() const;
  /// @brief Get X value (meters)
  /// @returns _x_m X value (meters)
  const double& x_m() const {return x_m_;}
  /// @brief Get Y value (meters)
  /// @returns _y_m Y value (meters)
  const double& y_m() const {return y_m_;}
  /// @brief Get z_value (meters)
  /// @returns _z_m Z value (meters)
  const double& z_m() const {return z_m_;}
  /// @brief Get maginitude (meters)
  /// @returns _r_m Magnutude (meters)
  const double& r_m() const {return r_m_;}
  /// @brief Get the distance from z-axis in the x/y-plane (meters)
  /// @returns _rho_rad Distance from z-axis in the x/y-plane (meters)
  const double& rho_m() const {return rho_m_;}
  /// @brief Get angle from the positive z-axis (radians)
  /// @returns _theta_rad Angle from the positive z-axis (radians)
  const double& theta_rad() const {return theta_rad_;}
  /// @brief Get angle from the positive x-axis (radians)
  /// @returns _phi_rad Angle from the positive x-axis (radians)
  const double& phi_rad() const {return phi_rad_;}

 private:
  /// @brief X value (meters)
  double x_m_;
  /// @brief Y value (meters)
  double y_m_;
  /// @brief Z value (meters)
  double z_m_;
  /// @brief Magnitude (meters)
  double r_m_;
  /// @brief Distance from z-axis in the x/y-plane (meters)
  double rho_m_;
  /// @brief Angle from the positive z-axis
  double theta_rad_;
  /// @brief Angle from the positive x-axis
  double phi_rad_;
};

/// Output vector to a stream
/// @param[in,out] strm stream to output to
/// @param[in] _vector Vector
inline std::ostream& operator<<(std::ostream& strm, const Vector& _vector) {
  return strm << _vector.ToString();
}

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_VECTOR_H_
