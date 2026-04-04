#include "drone.h"
#include "backend.h"
#include "common/id.h"

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using std::cout;
using std::endl;
using std::vector;

const int NUM_OF_DRONES = 1000;

int main() {
  DroneList fleet;
  vector<Drone> droneRegistry;

  cout << "Simulation Starting Up..." << endl;

  cout << "Adding drones..." << endl;
  for (int i = 0; i < NUM_OF_DRONES; i++) {
    Drone d;
    droneRegistry.push_back(d);
    fleet.addDrone(d);
    cout << "[New Drone Registered] Drone ID:" << d.getId() << endl;
  }
  cout << "Added drones completed." << endl;
  cout << "Fleet size: " << fleet.size() << endl;

  cout << "\nSimulation Begin\n" << endl;

  for (int tick = 0; tick < 100; tick++) {
    for (auto& d : droneRegistry) {
      d.movePos(0.001, 0.0015, 0.0);
      d.drainBattery(1);

      if (tick == 0) d.setState(TAKEOFF);
      else if (tick < 30) d.setState(CRUISE);
      else if (tick < 40) d.setState(APPROACH);
      else if (tick < 50) d.setState(DELIVERY);
      else d.setState(RETURNING);

      DroneState snapshot(d);
      fleet.update(snapshot);
    }
    int result = fleet.writeTelemetry("../backend/telemetry.json");
    if (result != 0) cout << "Failed to write telemetry\n";
    cout << "Tick " << tick << " written to telemetry.json" << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}