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

#include <cstdint>
#include <string>
#include <vector>

#include "collaborate/antenna_dipole.h"
#include "collaborate/antenna_helical.h"
#include "collaborate/battery.h"
#include "collaborate/data_logger.h"
#include "collaborate/data_processor_sink.h"
#include "collaborate/data_processor_source.h"
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
void AdvancedObservingSystemSimulation() {
  // Simulation Parameters
  constexpr uint64_t kNumTicks = 3600;
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
  DataProcessorSource source;
  DataProcessorSink sink;
  Battery battery(0.9333, 6, 12.9, 85);
  SolarPanel panel(29, 0.06, 0, 0, 0, &sun);
  SubsystemPower power_ss(battery, {panel, panel}, 6.2425);
  AntennaDipole comm_antenna(30, 0, 0, 0);
  ModemUhfDeploy uhf_modem;
  SubsystemComm comm(&comm_antenna, &uhf_modem);
  AntennaHelical sensing_antenna(30, 0, 0, 0);
  SensorCloudRadar cloud_radar("input/nc4/", 300);
  SubsystemSensing cloud(&sensing_antenna, &cloud_radar);
  SensorRainRadar rain_radar("input/nc4/", 50);
  SubsystemSensing rain(&sensing_antenna, &rain_radar);

  // Launch
  std::string tle_path = "input/tle/cubesat.tle";
  std::vector<PlatformOrbit> orbits = PlatformOrbitList(tle_path);
  //  std::vector<PlatformOrbit> C0 = {orbits[0]};
  std::vector<PlatformOrbit> C0 = orbits[0].Duplicate(1, 5, 1, 1, 0, 0);
  system.Launch(C0, 0, false, comm, cloud, power_ss, &source, &data_log);
  std::vector<PlatformOrbit> C1 = orbits[14].Duplicate(11, 11, 1, 1, 0, 0);
  system.Launch(C1, 1, false, comm, rain, power_ss, &sink, &data_log);

  // Final Setup
  data_log.Simulation(system.nodes().size(), kNumTicks);
  net_log.UnweightedNetwork(system.nodes().size(), kNumTicks);
  event_log.Initialize("info", "info", true);
  system.SeedMany(kNumTicks, 0);

  // Simulation
  for (uint64_t f = 0; f < kNumTicks; ++f) {
    system.Update();
    clock.Tick(kSecondsPerTick);
  }
  system.Complete();
  clock.Flush();
}
}  // namespace collaborate
}  // namespace osse

int main() {
  osse::collaborate::AdvancedObservingSystemSimulation();
  return 0;
}
