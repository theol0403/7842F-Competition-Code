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
  ObjDrawer drawer(lv_scr_act());
  drawer.withLayer(reader).withStyle(VISION_OBJECT_ERR_SIG, LV_COLOR_YELLOW).withStyle(1, LV_COLOR_BLUE).withStyle(2, LV_COLOR_RED);

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
