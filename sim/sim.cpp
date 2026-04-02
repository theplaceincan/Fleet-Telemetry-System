#include "sim.h"
#include "backend.h"
#include <iostream>
// using std::cout;
// using std::endl;
using namespace std;

const int NUM_OF_DRONES = 1000;

int generateUID() {
  static int staticId = 10000;
  return ++staticId;
}

int main() {
  DroneList fleet;
  cout << "Simulation Begin" << endl;

  cout << "Adding drones..." << endl;
  for (int i = 0; i < NUM_OF_DRONES; i++) {
    Drone d;
    fleet.addDrone(d);
    cout << "[New Drone Registered] Drone ID:" << d.getId() << endl;
  }
  cout << "Added drones completed." << endl;
  cout << "Fleet size: " << fleet.size() << endl;
}