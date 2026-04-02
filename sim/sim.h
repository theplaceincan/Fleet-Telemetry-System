#ifndef SIM_H
#define SIM_H

enum STATES {
  OFF,
  START,
  FLY,
  RETURNING,
  LANDED
};

struct Position {
  double lat;
  double lng;
  double alt;
};

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

};

int generateUID(void);

#endif