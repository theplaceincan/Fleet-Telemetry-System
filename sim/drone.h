#ifndef DRONE_H
#define DRONE_H

#include "../common/types.h"
#include "../common/id.h"

class Drone {
  private:
    int id;
    Position pos;
    int battery;
    STATES state;
  public:
    Drone()
    : id(generateUID()),
      pos{0.0, 0.0, 0.0},
      battery(100),
      state(OFF) {}

    int getId() const { return id; }
    Position getPosition() const { return pos; }
    int getBattery() const { return battery; }
    STATES getState() const { return state; }

    void movePos(double dLat, double dLng, double dAlt) {
      pos.lat += dLat;
      pos.lng += dLng;
      pos.alt += dAlt;
    }
    void drainBattery(int amount) {
      battery-=amount;
      if (battery < 0) battery = 0;
    };
    void setState(STATES newState) {state = newState;};
    void setPosition(double lat, double lng, double alt) {
      pos.lat = lat;
      pos.lng = lng;
      pos.alt = alt;
    }
};

#endif