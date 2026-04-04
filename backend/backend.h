#ifndef BACKEND_H
#define BACKEND_H

#include <chrono>
#include <cstddef>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>

#include "../common/types.h"
#include "../sim/drone.h"

inline const char* stateToString(STATES state) {
  switch (state) {
    case OFF: return "OFF";
    case START: return "START";
    case TAKEOFF: return "TAKEOFF";
    case CRUISE: return "CRUISE";
    case APPROACH: return "APPROACH";
    case DELIVERY: return "DELIVERY";
    case RETURNING: return "RETURNING";
    case LANDED: return "LANDED";
    default: return "UNKNOWN";
  }
}

class DroneState {
 private:
  int id;
  Position pos;
  int battery;
  STATES state;
  std::chrono::steady_clock::time_point last_updated;

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
        state(d.getState()),
        last_updated(std::chrono::steady_clock::now()) {}

  DroneState(int id, Position pos, int battery, STATES state)
      : id(id),
        pos(pos),
        battery(battery),
        state(state),
        last_updated(std::chrono::steady_clock::now()) {}

  int getId() const { return id; };
  Position getPosition() const { return pos; };
  int getBattery() const { return battery; };
  STATES getState() const { return state; };
};

class DroneList {
 private:
  std::unordered_map<int, DroneState> drones;

 public:
  DroneList() {};

  size_t size() const { return drones.size(); };

  void addDrone(const Drone& d) {
    DroneState dc(d);
    auto res = drones.emplace(dc.getId(), dc);
    if (!res.second)
      std::cout << "Line 47 in /backend.h, addDrone(): Drone already exists\n";
  }

  int update(const DroneState& state) {
    auto ditr = drones.find(state.getId());
    if (ditr == drones.end()) return -1;
    ditr->second = state;
    return 0;
  }

  const DroneState* getDroneState(int id) const {
    auto ditr = drones.find(id);
    if (ditr == drones.end()) return nullptr;
    return &ditr->second;
  }

  int writeTelemetry(const std::string& filename = "telemetry.json") const {
    std::ofstream file(filename);
    if (!file.is_open()) return -1;

    file << "{\n";
    file << "  \"drones\": [\n";

    bool first = true;

    for (const auto& pair : drones) {
      const DroneState& drone = pair.second;
      Position p = drone.getPosition();

      if (!first) {
        file << ",\n";
      }

      file << "    {\n";
      file << "      \"id\": " << drone.getId() << ",\n";
      file << "      \"position\": {\n";
      file << "        \"lat\": " << p.lat << ",\n";
      file << "        \"lng\": " << p.lng << ",\n";
      file << "        \"alt\": " << p.alt << "\n";
      file << "      },\n";
      file << "      \"battery\": " << drone.getBattery() << ",\n";
      file << "      \"state\": \"" << stateToString(drone.getState())
           << "\"\n";
      file << "    }";

      first = false;
    }

    file << "\n  ]\n";
    file << "}\n";

    file.close();
    return 0;
  }
};

#endif