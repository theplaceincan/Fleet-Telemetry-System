#include <sim.h>

int generateUID() {
  static int staticId = 10000;
  return ++staticId;
}

int main() {

}