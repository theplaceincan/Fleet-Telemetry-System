#ifndef BACKEND_H
#define BACKEND_H

#include <unordered_map>
#include "sim.h"

using std::unordered_map;

class DroneState {
  private:
    int id;
    Position pos;
    int battery;
    STATES state;
  public:
    DroneState(const Drone& d) 
    : id(d.getId()),
      pos(d.getPosition()),
      battery(d.getBattery()),
      state(d.getState()) {}
    
    int getId() const {return id;};
};

class DroneList {
  private:
    unordered_map<int, DroneState> drones;
  public:
    DroneList() {};
    
    int size() const {return drones.size();};
    
    void addDrone(const Drone& d) {
      DroneState dc(d);
      drones[dc.getId()] = dc;
    }

    int update(const DroneState& state) {
      if (drones.find(state.getId()) == drones.end()) return -1;
      drones[state.getId()] = state;
      return 0;
    }
};

#endif