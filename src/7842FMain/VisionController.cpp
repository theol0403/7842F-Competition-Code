#include "VisionController.hpp"

using namespace lib7842;

VisionController::VisionController(pros::Vision* isensor) :
sensor(isensor), reader(sensor),
visionTask(task, this)
{
}


void VisionController::run()
{

  sensor->set_exposure(83);
  pros::vision_signature_s_t SIG_1 = pros::Vision::signature_from_utility(1, -2657, -2461, -2559, 4939, 5765, 5352, 7.700, 0); sensor->set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 = pros::Vision::signature_from_utility(2, 6425, 7147, 6786, -329, -103, -216, 5.800, 0); sensor->set_signature(2, &SIG_2);
  ObjDrawer drawer(lv_scr_act());
  drawer.withStyle(LV_COLOR_HEX(0xFF7F00), LV_COLOR_WHITE);
  drawer.withLayer(reader).withStyle(VISION_OBJECT_ERR_SIG, LV_COLOR_YELLOW, LV_COLOR_WHITE).withStyle(1, LV_COLOR_BLUE, LV_COLOR_WHITE).withStyle(2, LV_COLOR_RED, LV_COLOR_WHITE);

  while(true)
  {
    reader.reset();
    reader.getAll();
    reader.print();
    drawer.draw();
    pros::delay(100);
  }
}


void VisionController::task(void* input)
{
  pros::delay(500);
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
