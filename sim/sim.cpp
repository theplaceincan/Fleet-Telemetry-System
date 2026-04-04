#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "backend/backend.h"
#include "common/id.h"
#include "data/locations.h"
#include "drone.h"
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> baseDist(0, BASES.size() - 1);
std::uniform_int_distribution<int> destDist(0, DESTINATIONS.size() - 1);

// Base
const double BASE_LAT = 36.1699;
const double BASE_LNG = -115.1398;
const double BASE_ALT = 0.0;

using std::cout;
using std::endl;
using std::vector;

const int NUM_OF_DRONES = 1000;

double distance2D(double lat1, double lng1, double lat2, double lng2) {
  double dLat = lat2 - lat1;
  double dLng = lng2 - lng1;
  return std::sqrt(dLat * dLat + dLng * dLng);
}

int main() {
  DroneList fleet;
  vector<Drone> droneRegistry;
  droneRegistry.reserve(NUM_OF_DRONES);

  cout << "Simulation Starting Up..." << endl;
  cout << "Adding drones..." << endl;
  for (int i = 0; i < NUM_OF_DRONES; i++) {

    Location chosenBase = BASES[baseDist(gen)];
    Location chosenDestination = DESTINATIONS[destDist(gen)];
    double latOffset = (i % 20) * 0.00005;
    double lngOffset = (i / 20) * 0.00005;

    Drone d(chosenBase, chosenDestination, latOffset, lngOffset);

    droneRegistry.push_back(d);
    fleet.addDrone(d);

    cout << "[New Drone Registered] Drone ID: " << d.getId()
         << " | Base: " << chosenBase.addr
         << " | Destination: " << chosenDestination.addr << endl;
  }
  cout << "Added drones completed." << endl;
  cout << "Fleet size: " << fleet.size() << endl;
  cout << "\nSimulation Begin\n" << endl;

  int tick = 0;
  while (true) {
    for (auto& d : droneRegistry) {
      Position p = d.getPosition();
      Location dest = d.getDestination();

      double dLat = dest.lat - p.lat;
      double dLng = dest.lng - p.lng;
      double dist = std::sqrt(dLat * dLat + dLng * dLng);

      if (dist < 0.0001) {
        d.setState(LANDED);
      } else {
        if (p.alt < 30.0) {
          d.setState(TAKEOFF);
          d.movePos(0.0, 0.0, 5.0);
        } else {
          d.setState(CRUISE);

          double step = 0.0003;
          double unitLat = dLat / dist;
          double unitLng = dLng / dist;

          d.movePos(unitLat * step, unitLng * step, 0.0);
        }
      }

      d.drainBattery(1);
      fleet.update(DroneState(d));
    }
    int result = fleet.writeTelemetry("backend/telemetry.json");
    if (result != 0) cout << "Failed to write telemetry\n";
    cout << "Tick " << tick++ << " written to telemetry.json" << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
}