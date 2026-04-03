#ifndef BACKEND_H
#define BACKEND_H

#include <unordered_map>
#include "sim.h"
#include <cstddef>

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

    DroneState(int id, Position pos, int battery, STATES state)
    : id(id),
      pos(pos),
      battery(battery),
      state(state) {}
    
    int getId() const {return id;};
    Position getPosition() const {return pos;};
    int getBattery() const {return battery;};
    STATES getState() const {return state;};
};

class DroneList {
  private:
    std::unordered_map<int, DroneState> drones;
  public:
    DroneList() {};
    
    size_t size() const {return drones.size();};
    
    void addDrone(const Drone& d) {
      DroneState dc(d);
      auto res = drones.emplace(dc.getId(), dc);
      if (!res.second) std::cout << "Line 47 in /backend.h, addDrone(): Drone already exists\n";
    }

    int update(const DroneState& state) {
      auto ditr = drones.find(state.getId());
      if (ditr == drones.end()) return -1;
      ditr->second = state;
      return 0;
    }
};

#endif