#ifndef BACKEND_H
#define BACKEND_H

#include <unordered_map>
#include "sim.h"

using std::unordered_map;

class DroneList {
  public:
    unordered_map<int, Drone> drones;
};

#endif