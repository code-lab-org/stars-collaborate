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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_UTIL_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_UTIL_H_

#include <algorithm>
#include <climits>
#include <cmath>
#include <complex>
#include <cstdint>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace osse {
namespace collaborate {
namespace util {

/// @brief Double-precision complex number
typedef std::complex<double> Complex;

/// @brief Double-precision complex polynomial
typedef std::vector<Complex> Polynomial;

/// @brief Pi (radians)
constexpr double kPiRad = M_PI;

/// @brief Pi divided by 2 (radians)
constexpr double kPiByTwoRad = M_PI_2;

/// @brief Pi multiplied by 2 (radians)
constexpr double kTwoPiRad = 2 * M_PI;

/// @brief Number of bits per byte
constexpr double kBitsPerByte = CHAR_BIT;

/// @file util.h
/// @brief Provides numeric constants and utilities

/// @fn std::string StringFromDouble(const double& _input,
///                                  const uint8_t& _characteristic,
///                                  const uint8_t& _mantissa)
/// @brief Produces a string from a double-precision number
/// @param[in] _input Input double-precision number
/// @param[in] _characteristic Number of digits before the decimal point
/// @param[in] _mantissa Number of digits after the decimal point
/// @returns String representation
inline std::string StringFromDouble(const double& _input,
                                    const uint8_t& _characteristic,
                                    const uint8_t& _mantissa) {
  std::stringstream stream;
  stream << std::setprecision(_mantissa)
         << std::fixed
         << std::setw(_characteristic + _mantissa + 1)
         << std::setfill('0')
         << std::internal
         << _input;
  return stream.str();
}

/// @fn uint64_t RandomNumberBetween(const uint64_t& _minimum,
///                                  const uint64_t& _maximum)
/// @brief Generates a random number between two values (inclusive)
/// @param[in] _minimum The minimum number
/// @param[in] _maximum The maximum number
/// @returns Random number
inline uint64_t RandomNumberBetween(const uint64_t& _minimum,
                                    const uint64_t& _maximum) {
  std::random_device seeder;
  std::mt19937 engine(seeder());
  std::uniform_int_distribution<int> distribution(_minimum, _maximum);
  return distribution(engine);
}

/// @fn uint64_t Index(const uint64_t& _row,
///                    const uint64_t& _column,
///                    const uint64_t& _num_columns)
/// @brief Calculates the index in a 1-D array as if it were 2-D
/// @param[in] _row Row
/// @param[in] _column Column
/// @param[in] _num_columns Number of columns
/// @returns Index
/// @details
inline uint64_t Index(const uint64_t& _row,
                      const uint64_t& _column,
                      const uint64_t& _num_columns) {
  return (_row * _num_columns + _column);
}

/// @brief Finds minimum positive root of a truncated polynomial (4th degree)
/// @param[in] _coefficients Coefficients of polynomial
/// @returns Minimum positive root of a truncated polynomial (4th degree)
double MinimumRoot(const Polynomial& _coefficients);

/// @brief Finds the complex roots of a truncated polynomial (4th degree)
/// @param[in] _coefficients Coefficients of polynomial
/// @returns Complex roots of a polynomial
Polynomial ComplexRoots(const Polynomial& _coefficients);

/// @brief Provides the acute angle between two proper real lines
/// @param[in] _coefficients Coefficients of polynomial
/// @param[in] _degree Degree of polynomial
/// @param[in] _value Current root
void Laguerre(const Polynomial& _coefficients,
              const int& _degree,
              Complex* _value);

}  // namespace util
}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_UTIL_H_
