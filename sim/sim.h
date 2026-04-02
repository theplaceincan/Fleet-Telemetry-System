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
  int lat;
  int lng;
};

class Drone {
  private:
    int id;
    Position pos;
    float battery;
    STATES state;
  public:
    Drone()
    : id(generateUID()),
      pos{0,0},
      battery(100.0f),
      state(OFF) {}

    int getId() const { return id; }
    Position getPosition() const { return pos; }
    float getBattery() const { return battery; }
    STATES getState() const { return state; }

};

int generateUID(void);

#endif