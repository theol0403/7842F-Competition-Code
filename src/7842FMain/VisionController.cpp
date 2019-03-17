#include "VisionController.hpp"

using namespace lib7842;

VisionController::VisionController(pros::Vision* isensor) :
sensor(isensor), reader(sensor),
visionTask(task, this)
{
}


void VisionController::run()
{

  sensor->set_exposure(84);
  pros::vision_signature_s_t SIG_1 = pros::Vision::signature_from_utility(1, -2185, -1577, -1882, 991, 1395, 1192, 4.400, 0); sensor->set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 = pros::Vision::signature_from_utility(2, 10067, 10571, 10318, -3227, -2823, -3024, 10.700, 0); sensor->set_signature(2, &SIG_2);

  while(true)
  {
    reader.reset();
    reader.getAll();
    lib7842::ObjectContainer sorter = reader;
    sorter = sorter.copy().removeWithout(objAttr::sig, 1).trim(4) + sorter.copy().removeWithout(objAttr::sig, 2).trim(4);
    sorter.print();
    pros::delay(100);
  }
}


void VisionController::task(void* input)
{
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
