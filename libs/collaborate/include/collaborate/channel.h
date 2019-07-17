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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_CHANNEL_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_CHANNEL_H_

#include <vector>

#include "collaborate/node.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/vector.h"

namespace osse {
namespace collaborate {

/// @class Channel
/// @brief Describes properties of a communication channel between nodes
class Channel {
 public:
  /// @brief A buffer for logged node data
  typedef struct LogBuffer {
    /// @brief Elapsed ticks
    std::vector<uint64_t> ticks;
    /// @brief Year
    std::vector<int> year;
    /// @brief Month
    std::vector<int> month;
    /// @brief Day
    std::vector<int> day;
    /// @brief Hour
    std::vector<int> hour;
    /// @brief Minute
    std::vector<int> minute;
    /// @brief Second
    std::vector<int> second;
    /// @brief Microsecond
    std::vector<int> microsecond;
    /// @brief Line of sight speed
    std::vector<double> los_speed;
    /// @brief Frequency
    std::vector<double> omega;
    /// @brief Distance
    std::vector<double> distance;
    /// @brief Delay
    std::vector<double> delay;
    /// @brief Data Rate
    std::vector<double> data_rate;
    /// @brief TX Index
    std::vector<double> tx_idx;
    /// @brief TX Buffer
    std::vector<uint64_t> tx_buffer;
    /// @brief TX Longitude
    std::vector<double> tx_lon;
    /// @brief TX Latitude
    std::vector<double> tx_lat;
    /// @brief TX Altitude
    std::vector<double> tx_alt;
    /// @brief TX Gain
    std::vector<double> tx_gain;
    /// @brief TX Power
    std::vector<double> tx_power;
    /// @brief RX Index
    std::vector<double> rx_idx;
    /// @brief RX Buffer
    std::vector<uint64_t> rx_buffer;
    /// @brief RX Longitude
    std::vector<double> rx_lon;
    /// @brief RX Latitude
    std::vector<double> rx_lat;
    /// @brief RX Altitude
    std::vector<double> rx_alt;
    /// @brief RX Gain
    std::vector<double> rx_gain;
    /// @brief RX Power
    std::vector<double> rx_power;
  } LogBuffer;
  /// @brief Speed of light in a vacuum (meters per second)
  static constexpr double kSpeedOfLightMPerS = 299792458.0;
  /// @brief Constructor
  /// @param[in] _tx_node Transmitter
  /// @param[in] _rx_node Receiver
  Channel(Node* _tx_node, Node* _rx_node);
  /// @brief Starts fake transmission of data
  void Start();
  /// @brief Updates member variables and performs fake transfers
  /// @param[in] _clock SimulationClock
  void Update(const SimulationClock& _clock);
  /// @brief Calculates expected transfer duration (seconds)
  /// @returns Expected transfer duration (seconds)
  uint64_t PredictTransferDurationS() const;
  /// @brief Calculates Expected transfer size (bytes)
  /// @returns Expected transfer size (bytes)
  uint64_t PredictTransferSizeBytes() const;
  /// @brief Get transmitter
  /// @returns tx_node_ Transmitter
  Node* tx_node() const {return tx_node_;}
  /// @brief Get receiver
  /// @returns rx_node_ Receiver
  Node* rx_node() const {return rx_node_;}
  /// @brief Get received gain (decibels)
  /// @returns rx_gain_db_ Received gain (decibels)
  const double& rx_gain_db() const {return rx_gain_db_;}
  /// @brief Get transmitted gain (decibels)
  /// @returns tx_gain_db_ Transmitted gain (decibels)
  const double& tx_gain_db() const {return tx_gain_db_;}
  /// @brief Get activity
  /// @returns active_ activity
  const bool& active() const {return active_;}
  /// @brief Get error status
  /// @returns error_flag_ Error status
  const bool& error_flag() const {return error_flag_;}
  /// @brief Get success status
  /// @returns success_flag_ Success status
  const bool& success_flag() const {return success_flag_;}
  /// @brief Get potential contact status
  /// @returns open_ Potential contact status
  const bool& open() const {return open_;}

 private:
  /// @brief Uses fake data buffers to simulate the movement of data
  /// @param[in] _clock Simulation clock
  void FakeTransfer(const SimulationClock& _clock);
  /// @brief Moves data from real transmit buffer to real receive buffer
  void RealTransfer();
  /// @brief Appends data to log buffer
  /// @param[in] _clock Simulation clock
  void Buffer(const SimulationClock& _clock);
  /// @brief Writes the log buffer to file
  /// @param[in] _clock Simulation clock
  void Flush(const SimulationClock& _clock);
  /// @brief Calculates data rate (bits per second)
  /// @returns Data rate (bits per second)
  /// @details
  /// \f[
  /// \delta = \min{(\delta_{tx}, \delta_{rx})} ~~~ (bits~per~second)
  /// \f]
  double CalculateDataRateBitsPerS() const;
  /// @brief Calculates frequency (radians per second)
  /// @returns Frequency (radians per second)
  /// @details
  /// \f[
  /// \omega = \max{(\omega_{tx}, \omega_{rx})} ~~~ (radians~per~second)
  /// \f]
  void UpdateOmegaRadPerS();
  /// @brief Updates distance (meters)
  /// @details
  /// \f[
  /// d = \left\lvert\vec{p}_{tx} - \vec{p}_{rx}\right\rvert ~~~ (meters)
  /// \f]
  void UpdateDistanceM();
  /// @brief Updates line-of-sight unit vectors
  /// @details
  /// \f[
  /// \vec{r}_{tx,rx} = \left\lvert \vec{p}_{rx} - \vec{p}_{tx} \right\rvert,~
  /// \vec{r}_{rx,tx} = \left\lvert \vec{p}_{tx} - \vec{p}_{rx} \right\rvert
  /// \f]
  /// \f[
  /// \hat{r}_{tx,rx} =
  ///     \frac{\vec{r}_{tx,rx}}{\left\lvert\vec{r}_{tx,rx}\right\rvert},~
  /// \hat{r}_{rx,tx} =
  ///     \frac{\vec{r}_{rx,tx}}{\left\lvert\vec{r}_{rx,tx}\right\rvert}
  /// \f]
  void UpdateLosUnit();
  /// @brief Updates line-of-sight speed (meters per second)
  /// @details
  /// \f[
  /// s = (\vec{v}_{t} - \vec{v}_{r}) \cdot \hat{p}_{tx,rx}
  /// ~~~ (meters~per~second)
  /// \f]
  void UpdateLosSpeedMPerS();
  /// @brief Updates directional gain values (decibels)
  /// @details
  /// \f[ (~for~both~TX~and~RX~nodes~) \f]
  /// \f[
  /// {\bf{O}}, {\bf{B}}, {\bf{A}} = Orbit,Body,Antenna~Frame~Attitude~Matrices
  /// \f]
  /// \f[ \hat{u} = LOS~Unit~Vector \f]
  /// \f[ \hat{a} = Antenna~LOS~Unit~Vector \f]
  /// \f[
  /// \hat{a} = \left(\left(\left(\hat{u}{\bf{O}^{-1}}\right)
  ///           {\bf{B}^{-1}}\right){\bf{A}^{-1}}\right)
  /// \f]
  /// \f[ g = G_{a}(\theta_{a}, \phi_{a}, \omega_{a}) ~~~ (decibels) \f]
  void UpdateGainDb();
  /// @brief Updates potential contact status
  /// @details
  /// \f[ \delta = Gain~Threshold \f]
  /// \f[ Open = (g_{tx} > \delta) \land (g_{rx} > \delta) \f]
  void UpdateOpen();
  /// @brief Updates power (Watts)
  /// @details
  /// \f[
  /// P_{rx} =
  /// P_{tx} * g_{tx} * g_{rx} * \frac{\frac{c}{\frac{\omega}{2\pi}}}{4\pi d}^2
  /// \f]
  void UpdatePowerW();
  /// @brief Updates delay (seconds)
  /// @details
  /// \f[ \delta = d / c \f]
  void UpdateDelayS();
  /// @brief Transmitter
  Node* tx_node_;
  /// @brief Receiver
  Node* rx_node_;
  /// @brief Data rate (bits per second)
  double data_rate_bits_per_s_;
  /// @brief Frequency (radians per second)
  double omega_rad_per_s_;
  /// @brief Received power (Watts)
  double rx_power_w_;
  /// @brief Received gain (decibels)
  double rx_gain_db_;
  /// @brief Received line-of-sight vector (unit)
  Vector rx_los_unit_;
  /// @brief Transmitted power (watts)
  double tx_power_w_;
  /// @brief Transmitted gain (decibels)
  double tx_gain_db_;
  /// @brief Transmitted line-of-sight vector (unit)
  Vector tx_los_unit_;
  /// @brief Line-of-sight speed (meters per second)
  double los_speed_m_per_s_;
  /// @brief Distance (meters)
  double distance_m_;
  /// @brief Delay (seconds)
  double delay_s_;
  /// @brief Activity status
  bool active_;
  /// @brief Fake receive buffer size (bytes)
  uint64_t fake_rx_buffer_bytes_;
  /// @brief Fake transmit buffer size (bytes)
  uint64_t fake_tx_buffer_bytes_;
  /// @brief Buffer for log data
  std::vector<double> log_buffer_;
  /// @brief Error status
  bool error_flag_;
  /// @brief Success status
  bool success_flag_;
  /// @brief Potential contact status
  bool open_;
  /// @brief Buffer for logged communication parameters
  LogBuffer log_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_CHANNEL_H_
