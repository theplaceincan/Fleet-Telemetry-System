#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "backend/backend.h"
#include "common/id.h"
#include "drone.h"

// Base
const double BASE_LAT = 36.1699;
const double BASE_LNG = -115.1398;
const double BASE_ALT = 0.0;

using std::cout;
using std::endl;
using std::vector;

const int NUM_OF_DRONES = 1000;

int main() {
  DroneList fleet;
  vector<Drone> droneRegistry;
  droneRegistry.reserve(NUM_OF_DRONES);

  cout << "Simulation Starting Up..." << endl;
  cout << "Adding drones..." << endl;
  for (int i = 0; i < NUM_OF_DRONES; i++) {
    Drone d;

    double latOffset = (i % 20) * 0.00005;
    double lngOffset = (i / 20) * 0.00005;
    d.setPosition(BASE_LAT + latOffset, BASE_LNG + lngOffset, BASE_ALT);

    droneRegistry.push_back(d);
    fleet.addDrone(d);

    cout << "[New Drone Registered] Drone ID:" << d.getId() << endl;
  }
  cout << "Added drones completed." << endl;
  cout << "Fleet size: " << fleet.size() << endl;
  cout << "\nSimulation Begin\n" << endl;

  int tick = 0;
  while (true) {
    int i = 0;
    for (auto& d : droneRegistry) {
      if (tick == 0) {
        d.setState(TAKEOFF);
        d.movePos(0.0, 0.0, 10.0);
      } else if (tick < 30) {
        d.setState(CRUISE);
        double dLat = 0.0005 + (i % 10) * 0.00002;
        double dLng = 0.0007 + (i % 15) * 0.00002;
        d.movePos(dLat, dLng, 0.0);
      } else if (tick < 40) {
        d.setState(APPROACH);
        d.movePos(0.0002, 0.0002, -2.0);
      } else if (tick < 50) {
        d.setState(DELIVERY);
      } else {
        d.setState(RETURNING);
        d.movePos(-0.0004, -0.0005, 0.0);
      }

      d.drainBattery(1);
      DroneState snapshot(d);
      fleet.update(snapshot);
      i++;
    }
    int result = fleet.writeTelemetry("backend/telemetry.json");
    if (result != 0) cout << "Failed to write telemetry\n";
    cout << "Tick " << tick++ << " written to telemetry.json" << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
}