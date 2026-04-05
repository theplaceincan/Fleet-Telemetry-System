#include "id.h"

int generateUID() {
  static int staticId = 100;
  return ++staticId;
}
