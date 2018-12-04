#include "OdomTask.hpp"

void driverOdomTask(void*)
{

  while (true) {
    auto state = robotChassis->getState();
    printf("x: %1.2f, y: %1.2f, theta: %1.2f\n",
           state.x.convert(inch),
           state.y.convert(inch),
           state.theta.convert(degree));
    pros::delay(50);
  }

}
