#include "OdomTask.hpp"

void odomTask(void*)
{
  std::cout << "tracking began" << std::endl;

  while(true) {
    chassisOdom->step();
    chassisOdom->debug();
    pros::delay(100);
  }


  std::cout << "tracking ended?" << std::endl;
}
