#include "VisionController.hpp"

using namespace lib7842;

VisionController::VisionController(pros::Vision* isensor) :
sensor(isensor), reader(sensor),
visionTask(task, this)
{
}


void VisionController::run()
{

  sensor->set_exposure(50);
  pros::vision_signature_s_t SIG_1 = pros::Vision::signature_from_utility(1, -2435, -2095, -2265, 6593, 7197, 6895, 4.700, 0); sensor->set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 = pros::Vision::signature_from_utility(2, 8085, 8613, 8350, -225, 1, -112, 10.400, 0); sensor->set_signature(2, &SIG_2);

  while(true)
  {
    reader.reset();
    reader.getAll();
    lib7842::ObjectContainer sorter = reader;
    sorter = sorter.copy().removeWithout(objAttr::sig, 2).trim(4);
    sorter.print();
    pros::delay(100);
  }
}


void VisionController::task(void* input)
{
  pros::delay(500);
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
