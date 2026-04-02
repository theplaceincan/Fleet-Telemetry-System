#ifndef BACKEND_H
#define BACKEND_H

#include <unordered_map>
#include "sim.h"

using std::unordered_map;

struct DroneState {
  int id;
  Position pos;
  float battery;
  STATES state;
};

class DroneList {
  private:
    unordered_map<int, Drone> drones;
  public:
    DroneList() {};

    void addDrone(const Drone& d) {
      drones[d.getId()] = d;
    }

    int size() const {return drones.size();};
    int update(const DroneState& state) {
      
    }
};

#endif