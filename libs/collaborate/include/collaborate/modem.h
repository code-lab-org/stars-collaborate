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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_MODEM_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_MODEM_H_

#include <cstdint>
#include <string>

namespace osse {
namespace collaborate {

/// @class Modem
/// @brief Abstract modem
class Modem {
 public:
  /// @brief Constructor
  /// @param[in] _kId String identifier for modem
  /// @param[in] _kRxRfPowerW RX RF power (watts)
  /// @param[in] _kRxRateBitsPerS RX data rate (bits per second)
  /// @param[in] _kRxOmegaRadPerS RX frequency (radians per second)
  /// @param[in] _kRxLambdaM RX wavelength (meters)
  /// @param[in] _kRxConsumedPowerW RX consumed power (watts)
  /// @param[in] _kTxRfPowerW TX RF power (watts)
  /// @param[in] _kTxRateBitsPerS TX data rate (bits per second)
  /// @param[in] _kTxOmegaRadPerS TX frequency (radians per second)
  /// @param[in] _kTxLambdaM TX wavelength (meters)
  /// @param[in] _kTxConsumedPowerW TX consumed power (watts)
  Modem(const std::string& _kId,
        const double& _kRxRfPowerW,
        const uint64_t& _kRxRateBitsPerS,
        const double& _kRxOmegaRadPerS,
        const double& _kRxLambdaM,
        const double& _kRxConsumedPowerW,
        const double& _kTxRfPowerW,
        const uint64_t& _kTxRateBitsPerS,
        const double& _kTxOmegaRadPerS,
        const double& _kTxLambdaM,
        const double& _kTxConsumedPowerW);
  /// @brief Get ID
  /// @returns kId_ Identifying string
  const std::string& kId() {return kId_;}
  /// @brief Get RX RF power (watts)
  /// @returns kRxRfPowerW_ RX RF power (watts)
  const double& kRxRfPowerW() const {return kRxRfPowerW_;}
  /// @brief Get RX data rate (bits per second)
  /// @returns kRxRateBitsPerS_ RX data rate (bits per second)
  const uint64_t& kRxRateBitsPerS() const {return kRxRateBitsPerS_;}
  /// @brief Get RX frequency (radians per second)
  /// @returns kRxOmegaRadPerS_ RX frequency (radians per second)
  const double& kRxOmegaRadPerS() const {return kRxOmegaRadPerS_;}
  /// @brief Get RX wavelength (meters)
  /// @returns kRxLambdaM_ RX wavelength (meters)
  const double& kRxLambdaM() const {return kRxLambdaM_;}
  /// @brief Get RX consumed power (watts)
  /// @returns kRxConsumedPowerW_ RX consumed power (watts)
  const double& kRxConsumedPowerW() const {return kRxConsumedPowerW_;}
  /// @brief Get TX RF power (watts)
  /// @returns kTxRfPowerW_ TX RF power (watts)
  const double& kTxRfPowerW() const {return kTxRfPowerW_;}
  /// @brief Get TX data rate (bits per second)
  /// @returns kTxRateBitsPerS_ TX data rate (bits per second)
  const uint64_t& kTxRateBitsPerS() const {return kTxRateBitsPerS_;}
  /// @brief Get TX frequency (radians per second)
  /// @returns kTxOmegaRadPerS_ TX frequency (radians per second)
  const double& kTxOmegaRadPerS() const {return kTxOmegaRadPerS_;}
  /// @brief Get TX wavelength (meters)
  /// @returns kTxLambdaM_ TX wavelength (meters)
  const double& kTxLambdaM() const {return kTxLambdaM_;}
  /// @brief Get TX consumed power (watts)
  /// @returns kTxConsumedPowerW_ TX consumed power (watts)
  const double& kTxConsumedPowerW() const {return kTxConsumedPowerW_;}

 protected:
  /// @brief Identifying string
  const std::string kId_;
  /// @brief RX RF power (watts)
  const double kRxRfPowerW_;
  /// @brief RX data rate (bits per second)
  const uint64_t kRxRateBitsPerS_;
  /// @brief RX frequency (radians per second)
  const double kRxOmegaRadPerS_;
  /// @brief RX wavelength (meters)
  const double kRxLambdaM_;
  /// @brief RX consumed power (watts)
  const double kRxConsumedPowerW_;
  /// @brief TX RF power (watts)
  const double kTxRfPowerW_;
  /// @brief TX data rate (bits per second)
  const uint64_t kTxRateBitsPerS_;
  /// @brief TX frequency (radians per second)
  const double kTxOmegaRadPerS_;
  /// @brief TX wavelength (meters)
  const double kTxLambdaM_;
  /// @brief TX consumed power (watts)
  const double kTxConsumedPowerW_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_MODEM_H_
