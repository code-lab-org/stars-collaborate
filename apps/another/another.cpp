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

#include <cstdint>
#include <string>
#include <vector>
#include <experimental/filesystem>

#include "collaborate/antenna_dipole.h"
#include "collaborate/antenna_helical.h"
#include "collaborate/battery.h"
#include "collaborate/data_logger.h"
#include "collaborate/data_processor_sink.h"
#include "collaborate/data_processor_source.h"
#include "collaborate/data_processor_template.h"
#include "collaborate/event_logger.h"
#include "collaborate/modem_uhf_deploy.h"
#include "collaborate/observing_system_alpha.h"
#include "collaborate/platform_orbit.h"
#include "collaborate/scheduler_alpha.h"
#include "collaborate/sensor_cloud_radar.h"
#include "collaborate/sensor_optical_imager.h"
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
  constexpr uint64_t kNumTicks = 500;
  constexpr uint64_t kSecondsPerTick = 1;

  // Loggers
  DataLogger data_log("output/data.nc4");
  EventLogger event_log("output/events.txt");
  DataLogger net_log("output/network.nc4");

  // Observing System
  SimulationClock clock(&data_log, 2021, 4, 20, 19, 0, 0);
  SchedulerAlpha scheduler(&clock);
  Sun sun(&clock);
  ObservingSystemAlpha system(&sun, &clock, &scheduler, &event_log, &net_log);

  // Satellite Hardware
  DataProcessorTemplate processor;
  Battery battery(0.9333, 6, 12.9, 85);
  SolarPanel panel(29, 0.06, 0, 0, 0, &sun);
  SubsystemPower power_ss(battery, {panel, panel}, 6.2425);
  AntennaDipole comm_antenna(30, 0, 0, 0);
  ModemUhfDeploy uhf_modem;
  SubsystemComm comm(&comm_antenna, &uhf_modem);
  AntennaHelical sensing_antenna(30, 0, 0, 0);
  SensorRainRadar rain_radar("input/nc4/", 30);
  SubsystemSensing rain(&sensing_antenna, &rain_radar);
  SensorOpticalImager optical_imager("input/nc4/", 30);
  SubsystemSensing optical(&sensing_antenna, &optical_imager);

  // Launch
  std::string tle_path1 = "input/tle/iss.tle";
  std::vector<PlatformOrbit> orbits1 = PlatformOrbitList(tle_path1);
  std::vector<PlatformOrbit> C1 = orbits1[0].Duplicate(1, 8, 1, 1, 0, 0);
  system.Launch(C1, 0, false, comm, rain, power_ss, &processor, &data_log);
  std::string tle_path2 = "input/tle/planetlabs.tle";
  std::vector<PlatformOrbit> orbits2 = PlatformOrbitList(tle_path2);
  system.Launch(orbits2, 1, false, comm, optical, power_ss, &processor, &data_log);
  
  // std::string tle_path2 = "input/tle/iss.tle";
  // std::vector<PlatformOrbit> orbits2 = PlatformOrbitList(tle_path2);
  // std::vector<PlatformOrbit> C2 = orbits2[0].Duplicate(10, 5, 1, 1, 0, 0);
  // system.Launch(C2, 1, false, comm, rain, power_ss, &processor, &data_log);
  // std::string tle_path3 = "input/tle/planetlabs.tle";
  // std::vector<PlatformOrbit> orbits3 = PlatformOrbitList(tle_path3);
  // std::vector<PlatformOrbit> C3 = orbits3[0].Duplicate(10, 5, 1, 1, 0, 0);
  // system.Launch(C3, 2, false, comm, optical, power_ss, &processor, &data_log);

  // Final Setup
  data_log.Simulation(system.nodes().size(), kNumTicks);
  net_log.UnweightedNetwork(system.nodes().size(), kNumTicks);
  event_log.Initialize("info", "info", true);
  system.Seed(kNumTicks);

  // Simulation
  for (uint64_t tick = 0; tick < kNumTicks; ++tick) {
    system.Update();
    clock.Tick(kSecondsPerTick);
  }
  system.Complete();
  clock.Flush();
}

void CollaborativeObservingSystemSimulation() {
  // Simulation Parameters
  constexpr uint64_t kNumTicks = 10000;
  constexpr uint64_t kSecondsPerTick = 1;

  // Loggers
  DataLogger data_log("output/data.nc4");
  EventLogger event_log("output/events.txt");
  DataLogger net_log("output/network.nc4");

  // Observing System
  SimulationClock clock(&data_log, 2021, 4, 20, 19, 0, 0);
  SchedulerAlpha scheduler(&clock);
  Sun sun(&clock);
  ObservingSystemAlpha system(&sun, &clock, &scheduler, &event_log, &net_log);

  // Satellite Hardware
  DataProcessorSource source;
  DataProcessorSink sink;
  DataProcessorTemplate processor;
  Battery battery(0.9333, 6, 12.9, 85);
  SolarPanel panel(29, 0.06, 0, 0, 0, &sun);
  SubsystemPower power_ss(battery, {panel, panel}, 6.2425);
  AntennaDipole comm_antenna(30, 0, 0, 0);
  ModemUhfDeploy uhf_modem;
  SubsystemComm comm(&comm_antenna, &uhf_modem);
  AntennaHelical sensing_antenna(30, 0, 0, 0);
  SensorCloudRadar cloud_radar("input/nc4/", 120);
  SubsystemSensing cloud(&sensing_antenna, &cloud_radar);
  SensorRainRadar rain_radar("input/nc4/", 30);
  SubsystemSensing rain(&sensing_antenna, &rain_radar);
  SensorOpticalImager optical_imager("input/nc4/", 30);
  SubsystemSensing optical(&sensing_antenna, &optical_imager);

  // Collaborative
  std::string tle_path1 = "input/tle/cloudsat.tle";
  std::vector<PlatformOrbit> C1 = PlatformOrbitList(tle_path1);
  std::vector<PlatformOrbit> C1b = C1[0].Duplicate(5, 10, 1, 1, 0, 0);
  system.Launch(C1b, 0, false, comm, cloud, power_ss, &source, &data_log);
  std::string tle_path2 = "input/tle/iss.tle";
  std::vector<PlatformOrbit> orbits2 = PlatformOrbitList(tle_path2);
  std::vector<PlatformOrbit> C2 = orbits2[0].Duplicate(10, 5, 1, 1, 0, 0);
  system.Launch(C2, 1, false, comm, rain, power_ss, &sink, &data_log);
  std::string tle_path3 = "input/tle/planetlabs.tle";
  std::vector<PlatformOrbit> orbits3 = PlatformOrbitList(tle_path3);
  std::vector<PlatformOrbit> C3 = orbits3[0].Duplicate(10, 5, 1, 1, 0, 0);
  system.Launch(C3, 2, false, comm, optical, power_ss, &sink, &data_log);
  
  // Final Setup
  data_log.Simulation(system.nodes().size(), kNumTicks);
  net_log.UnweightedNetwork(system.nodes().size(), kNumTicks);
  event_log.Initialize("info", "info", true);
  system.SeedMany(kNumTicks, 0);

  // Simulation
  for (uint64_t tick = 0; tick < kNumTicks; ++tick) {
    system.Update();
    clock.Tick(kSecondsPerTick);
  }
  system.Complete();
  clock.Flush();
}

void SecondCollaborativeObservingSystemSimulation() {
  // Simulation Parameters
  constexpr uint64_t kNumTicks = 10000;
  constexpr uint64_t kSecondsPerTick = 1;
  constexpr bool exceptional = true;

  // Loggers
  DataLogger data_log("output/data.nc4");
  EventLogger event_log("output/events.txt");
  DataLogger net_log("output/network.nc4");

  // Observing System
  SimulationClock clock(&data_log, 2021, 4, 20, 19, 0, 0);
  SchedulerAlpha scheduler(&clock, exceptional);
  Sun sun(&clock);
  ObservingSystemAlpha system(&sun, &clock, &scheduler, &event_log, &net_log,
                              exceptional);

  // Satellite Hardware
  DataProcessorSource source(exceptional);
  DataProcessorSink sink;
  DataProcessorTemplate processor;
  Battery battery(0.9333, 6, 12.9, 85);
  SolarPanel panel(29, 0.06, 0, 0, 0, &sun);
  SubsystemPower power_ss(battery, {panel, panel}, 6.2425);
  AntennaDipole comm_antenna(30, 0, 0, 0);
  ModemUhfDeploy uhf_modem;
  SubsystemComm comm(&comm_antenna, &uhf_modem);
  AntennaHelical sensing_antenna(30, 0, 0, 0);
  SensorCloudRadar cloud_radar("input/nc4/", 10);
  SubsystemSensing cloud(&sensing_antenna, &cloud_radar);
  SensorRainRadar rain_radar("input/nc4/", 30);
  SubsystemSensing rain(&sensing_antenna, &rain_radar);
  SensorOpticalImager optical_imager("input/nc4/", 30);
  SubsystemSensing optical(&sensing_antenna, &optical_imager);

  // Collaborative
  std::string tle_path1 = "input/tle/cloudsat.tle";
  std::vector<PlatformOrbit> C1 = PlatformOrbitList(tle_path1);
  // std::vector<PlatformOrbit> C1b = C1[0].Duplicate(4, 1, 1, 1, 0, 0);
  // system.Launch(C1b, 0, false, comm, cloud, power_ss, &source, &data_log);
  system.Launch(C1, 0, false, comm, cloud, power_ss, &source, &data_log);
  std::string tle_path2 = "input/tle/iss.tle";
  std::vector<PlatformOrbit> orbits2 = PlatformOrbitList(tle_path2);
  std::vector<PlatformOrbit> C2 = orbits2[0].Duplicate(1, 8, 1, 1, 0, 0);
  system.Launch(C2, 1, false, comm, rain, power_ss, &sink, &data_log);
  std::string tle_path3 = "input/tle/planetlabs.tle";
  std::vector<PlatformOrbit> C3 = PlatformOrbitList(tle_path3);
  system.Launch(C3, 2, false, comm, optical, power_ss, &sink, &data_log);
  
  // Final Setup
  data_log.Simulation(system.nodes().size(), kNumTicks);
  net_log.UnweightedNetwork(system.nodes().size(), kNumTicks);
  event_log.Initialize("info", "info", true);
  system.SeedManyMore(kNumTicks, 0);

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

void archive(std::string path, std::string directory) {
  std::experimental::filesystem::create_directory(path + "/" + directory);
  for (const auto & entry : std::experimental::filesystem::directory_iterator(path)) {
    if (!std::experimental::filesystem::is_directory(entry)
        && (entry.path() != path + "/.gitignore")){
      std::string from = entry.path();
      std::string root = entry.path().parent_path();
      std::string base = entry.path().filename();
      std::string to = root + "/" + directory + "/" + base;
      std::experimental::filesystem::rename(from, to);
    }
  }
}

int main() {
  osse::collaborate::SimpleObservingSystemSimulation();
  archive("output", "s");
  osse::collaborate::CollaborativeObservingSystemSimulation();
  archive("output", "c1");
  osse::collaborate::SecondCollaborativeObservingSystemSimulation();
  archive("output", "c2");
  return 0;
}
