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
std::uniform_real_distribution<double> offsetDist(-0.001, 0.001);
std::uniform_real_distribution<double> speedDist(0.0008, 0.0018); // faster for gif
std::uniform_int_distribution<int> batteryDist(30, 100); // staggered starting battery
std::uniform_int_distribution<int> stateDist(0, 2); // random starting state

using std::cout;
using std::endl;
using std::vector;

const int NUM_OF_DRONES = 100;
const double CRUISE_ALTITUDE = 30.0;
const int DELIVERY_WAIT_TICKS = 2;
const int CHARGE_PER_TICK = 20;   // charges faster so cycle is visible
const int BATTERY_DRAIN = 2;      // drains faster so returns are frequent
const int LOW_BATTERY_THRESHOLD = 20;

void tickDrone(Drone& d, int index, vector<int>& deliveryTicks) {
  Position p = d.getPosition();
  STATES state = d.getState();

  if (state == OFF) {
    d.setState(TAKEOFF);
  }

  else if (state == TAKEOFF) {
    if (p.alt < CRUISE_ALTITUDE) {
      d.movePos(0.0, 0.0, 5.0);
      d.drainBattery(1);
    } else {
      d.setState(CRUISE);
    }
  }

  else if (state == CRUISE) {
    if (d.getBattery() <= LOW_BATTERY_THRESHOLD) {
      d.setState(RETURNING);
      return;
    }

    Location dest = d.getDestination();
    double dLat = dest.lat - p.lat;
    double dLng = dest.lng - p.lng;
    double dist = std::sqrt(dLat * dLat + dLng * dLng);

    if (dist < 0.0005) {
      d.setPosition(dest.lat, dest.lng, p.alt);
      d.setState(DELIVERY);
      deliveryTicks[index] = DELIVERY_WAIT_TICKS;
    } else {
      double step = d.getSpeed();
      d.movePos((dLat / dist) * step, (dLng / dist) * step, 0.0);
      d.drainBattery(BATTERY_DRAIN);
    }
  }

  else if (state == DELIVERY) {
    if (deliveryTicks[index] > 0) {
      deliveryTicks[index]--;
    } else {
      d.setState(RETURNING);
    }
  }

  else if (state == RETURNING) {
    Location base = d.getBase();
    double dLat = base.lat - p.lat;
    double dLng = base.lng - p.lng;
    double dist = std::sqrt(dLat * dLat + dLng * dLng);

    if (dist > 0.0005) {
      double step = d.getSpeed();
      d.movePos((dLat / dist) * step, (dLng / dist) * step, 0.0);
      d.drainBattery(BATTERY_DRAIN);
    } else {
      d.setPosition(base.lat, base.lng, p.alt);
      if (p.alt > 0.0) {
        double descent = std::min(p.alt, 5.0);
        d.movePos(0.0, 0.0, -descent);
      } else {
        d.setState(LANDED);
      }
    }
  }

  else if (state == LANDED) {
    d.setState(CHARGING);
  }

  else if (state == CHARGING) {
    if (d.getBattery() < 100) {
      d.chargeBattery(CHARGE_PER_TICK);
    } else {
      Location newDest = DESTINATIONS[destDist(gen)];
      d.setDestination(newDest);
      Location base = d.getBase();
      d.setPosition(base.lat, base.lng, 0.0);
      d.setState(TAKEOFF);
    }
  }
}

int main() {
  DroneList fleet;
  vector<Drone> droneRegistry;
  vector<int> deliveryTicks;

  droneRegistry.reserve(NUM_OF_DRONES);
  deliveryTicks.resize(NUM_OF_DRONES, 0);

  cout << "Simulation Starting Up..." << endl;

  for (int i = 0; i < NUM_OF_DRONES; i++) {
    Location chosenBase = BASES[baseDist(gen)];
    Location chosenDest = DESTINATIONS[destDist(gen)];
    double latOffset = offsetDist(gen);
    double lngOffset = offsetDist(gen);

    Drone d(chosenBase, chosenDest, latOffset, lngOffset);
    d.setSpeed(speedDist(gen));

    // stagger battery so drones are at different lifecycle stages immediately
    int startBattery = batteryDist(gen);
    int batteryDiff = 100 - startBattery;
    d.drainBattery(batteryDiff);

    // stagger starting state so map is immediately active
    int startState = stateDist(gen);
    if (startState == 0) {
      // starts mid-cruise — position between base and dest
      double midLat = (chosenBase.lat + chosenDest.lat) / 2.0 + latOffset;
      double midLng = (chosenBase.lng + chosenDest.lng) / 2.0 + lngOffset;
      d.setPosition(midLat, midLng, CRUISE_ALTITUDE);
      d.setState(CRUISE);
    } else if (startState == 1) {
      // starts returning from destination
      d.setPosition(chosenDest.lat + latOffset, chosenDest.lng + lngOffset, CRUISE_ALTITUDE);
      d.setState(RETURNING);
    } else {
      // starts at takeoff
      d.setState(TAKEOFF);
    }

    droneRegistry.push_back(d);
    fleet.addDrone(d);

    cout << "[Registered] Drone #" << d.getId()
         << " | Base: " << chosenBase.addr
         << " | Dest: " << chosenDest.addr << endl;
  }

  cout << "Fleet size: " << fleet.size() << "\nSimulation Begin\n" << endl;

  int tick = 0;
  while (true) {
    for (int i = 0; i < NUM_OF_DRONES; i++) {
      tickDrone(droneRegistry[i], i, deliveryTicks);
      fleet.update(DroneState(droneRegistry[i]));
    }

    int result = fleet.writeTelemetry("backend/telemetry.json");
    if (result != 0) cout << "Failed to write telemetry\n";

    cout << "Tick " << tick++ << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(800)); // faster for gif
  }
}