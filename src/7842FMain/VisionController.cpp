#include "VisionController.hpp"

VisionController::VisionController(pros::Vision* isensor) :
sensor(isensor), reader(sensor),
visionTask(task, this)
{
}


void VisionController::run()
{

  pros::vision_signature_s_t SIG_1 = pros::Vision::signature_from_utility(1, -2137, -1661, -1899, 3099, 4065, 3582, 5.400, 0); sensor->set_signature(1, &SIG_1);

  while(true)
  {
    reader.reset();
    reader.getAll();
    reader.print();
    pros::delay(100);
  }
}


void VisionController::task(void* input)
{
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
