// Copyright (C) 2020 Andrew O'Brien
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

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "collaborate/antenna_dipole.h"
#include "collaborate/antenna_helical.h"
#include "collaborate/battery.h"
#include "collaborate/data_logger.h"
#include "collaborate/data_processor_source.h"
#include "collaborate/data_processor_template.h"
#include "collaborate/event_logger.h"
#include "collaborate/modem_uhf_deploy.h"
#include "collaborate/observing_system_alpha.h"
#include "collaborate/platform_orbit.h"
#include "collaborate/scheduler_alpha.h"
#include "collaborate/sensor_cloud_radar.h"
#include "collaborate/sensor_rain_radar.h"
#include "collaborate/simulation_clock.h"
#include "collaborate/solar_panel.h"
#include "collaborate/subsystem_comm.h"
#include "collaborate/subsystem_power.h"
#include "collaborate/subsystem_sensing.h"
#include "collaborate/sun.h"

namespace osse {
namespace collaborate {
void SimpleObservingSystemSimulation() {
  // Simulation Parameters
  constexpr uint64_t kNumTicks = 400;
  constexpr uint64_t kSecondsPerTick = 1;

  // Loggers
  DataLogger data_log("output/data.nc4");
  EventLogger event_log("output/events.txt");
  DataLogger net_log("output/network.nc4");

  // Observing System
  SimulationClock clock(&data_log);
  SchedulerAlpha scheduler(&clock);
  Sun sun(&clock);
  ObservingSystemAlpha system(&sun, &clock, &scheduler, &event_log, &net_log);

  // Satellite Hardware
  DataProcessorTemplate processor;
  Battery battery(0.9333, 6, 12.9, 85);
  SolarPanel panel(29, 0.06, 0, 0, 0, &sun);
  SubsystemPower power_ss(battery, {panel, panel}, 6.2425);
  AntennaDipole comm_antenna(30, 0, 0, 0);
  comm_antenna.Log("output/antenna.nc4");
  ModemUhfDeploy uhf_modem;
  SubsystemComm comm(&comm_antenna, &uhf_modem);
  AntennaHelical sensing_antenna(30, 0, 0, 0);
  SensorCloudRadar cloud_radar("input/nc4/", 10);
  SubsystemSensing cloud(&sensing_antenna, &cloud_radar);

  // GPM-CORE
  std::array<std::string, 3> tle_24 = {"GPM-CORE",
    "1 39574U 14009C   20312.76104295  .00004698  00000-0  72484-4 0  9990",
    "2 39574  65.0076  24.2122 0010842 281.7979  78.1951 15.55503858380338"};
  PlatformOrbit p_24 = PlatformOrbit(tle_24);
  std::vector<PlatformOrbit> c_24 = p_24.Duplicate(1, 3, 3, 1, 7, 0);
  system.Launch(c_24, 0, false, comm, cloud, power_ss, &processor, &data_log);

  // AQUA
  std::array<std::string, 3> tle_53 = {"AQUA",
    "1 27424U 02022A   20312.79398764  .00000128  00000-0  38470-4 0  9993",
    "2 27424  98.2043 251.3049 0000174 299.8763 144.9516 14.57114090984796"};
  PlatformOrbit p_53 = PlatformOrbit(tle_53);
  std::vector<PlatformOrbit> c_53 = p_53.Duplicate(4, 11, 1, 1, 0, 0);
  system.Launch(c_53, 1, false, comm, cloud, power_ss, &processor, &data_log);

  // CALIPSO
  std::array<std::string, 3> tle_75 = {"CALIPSO",
    "1 29108U 06016B   20312.84538492  .00000175  00000-0  44027-4 0  9998",
    "2 29108  98.2464 259.4918 0001363  80.9207 279.2148 14.62458176773275"};
  PlatformOrbit p_75 = PlatformOrbit(tle_75);
  std::vector<PlatformOrbit> c_75 = p_75.Duplicate(1, 5, 1, 2, 0, 7);
  system.Launch(c_75, 2, false, comm, cloud, power_ss, &processor, &data_log);

  // Final Setup
  data_log.Simulation(system.nodes().size(), kNumTicks);
  net_log.UnweightedNetwork(system.nodes().size(), kNumTicks);
  event_log.Initialize("info", "info", true);

  // Simulation
  for (uint64_t tick = 0; tick < kNumTicks; ++tick) {
    system.Update();
    clock.Tick(kSecondsPerTick);
  }
  system.Complete();
  clock.Flush();
}
}  // namespace collaborate
}  // namespace osse

int main() {
  osse::collaborate::SimpleObservingSystemSimulation();
  return 0;
}
