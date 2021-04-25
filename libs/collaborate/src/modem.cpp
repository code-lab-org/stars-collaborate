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

#include "collaborate/modem.h"

#include <cstdint>
#include <string>

namespace osse {
namespace collaborate {

Modem::Modem(const std::string& _id,
             const double& _rx_rf_power_w,
             const uint64_t& _rx_rate_bits_per_s,
             const double& _rx_omega_rad_per_s,
             const double& _rx_lambda_m,
             const double& _rx_consumed_power_w,
             const double& _tx_rf_power_w,
             const uint64_t& _tx_rate_bits_per_s,
             const double& _tx_omega_rad_per_s,
             const double& _tx_lambda_m,
             const double& _tx_consumed_power_w)
    : kId_(_id),
      kRxRfPowerW_(_rx_rf_power_w),
      kRxRateBitsPerS_(_rx_rate_bits_per_s),
      kRxOmegaRadPerS_(_rx_omega_rad_per_s),
      kRxLambdaM_(_rx_lambda_m),
      kRxConsumedPowerW_(_rx_consumed_power_w),
      kTxRfPowerW_(_tx_rf_power_w),
      kTxRateBitsPerS_(_tx_rate_bits_per_s),
      kTxOmegaRadPerS_(_tx_omega_rad_per_s),
      kTxLambdaM_(_tx_lambda_m),
      kTxConsumedPowerW_(_tx_consumed_power_w) {
}

}  // namespace collaborate
}  // namespace osse
