#include "VisionController.hpp"

VisionController::VisionController(ObjectReading* ivision) :
vision(ivision),
intakeTask(task, this)
{
}


void VisionController::run()
{
  while(true)
  {
    vision->reset();
    vision->getAll();
    vision->print();
    pros::delay(100);
  }
}


void VisionController::task(void* input)
{
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
