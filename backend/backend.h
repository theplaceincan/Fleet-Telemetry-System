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
    // DroneState()
    // : id(generateUID()),
    //   pos{0.0, 0.0, 0.0},
    //   battery(100),
    //   state(OFF) {}

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
    
    size_t size() const {return drones.size();};
    
    void addDrone(const Drone& d) {
      DroneState dc(d);
      drones.emplace(dc.getId(), dc);
    }

    int update(const DroneState& state) {
      if (drones.find(state.getId()) == drones.end()) return -1;
      auto ditr = drones.find(state.getId());
      ditr->second = state;
      return 0;
    }
};

#endif