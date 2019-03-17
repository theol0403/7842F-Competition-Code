#include "VisionController.hpp"

VisionController::VisionController(lib7842::ObjectReading* ivision) :
vision(ivision),
visionTask(task, this)
{
}


void VisionController::run()
{
  pros::delay(500);

  // pros::vision_signature_s_t SIG_1 = pros::Vision::signature_from_utility(1, -2137, -1661, -1899, 3099, 4065, 3582, 5.400, 0);
  // std::cerr << (vision->vision == nullptr) << "\n";
  //vision->vision->set_signature(1, &SIG_1);

  // pros::Vision vision(4);
  // pros::vision_signature_s_t SIG_1 = pros::Vision::signature_from_utility(1, -2137, -1661, -1899, 3099, 4065, 3582, 5.400, 0); vision.set_signature(1, &SIG_1);

  vision->vision->set_led(COLOR_DARK_RED);


  while(true)
  {
    // vision->reset();
    // vision->getAll();
    // vision->print();
    pros::delay(100);
  }
}


void VisionController::task(void* input)
{
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
