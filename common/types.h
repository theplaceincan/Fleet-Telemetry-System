#ifndef TYPES_H
#define TYPES_H

enum STATES {
  OFF,
  START,
  TAKEOFF,
  CRUISE,
  APPROACH,
  DELIVERY,
  RETURNING,
  LANDED
};

struct Position {
  double lat;
  double lng;
  double alt;
};

#endif