#include "VisionController.hpp"

using namespace lib7842;

VisionController::VisionController(pros::Vision* isensor, lv_obj_t* iparent) :
sensor(isensor), parent(iparent),
visionTask(task, this)
{
}


void VisionController::run()
{

  sensor->set_exposure(150);
  pros::vision_signature_s_t SIG_1 = {1, {1, 0, 0}, 2.800, -2129, -1369, -1749, 3221, 4401, 3811, 0, 0}; sensor->set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 = {2, {1, 0, 0}, 7.600, 6823, 7385, 7104, -2009, -1543, -1776, 0, 0}; sensor->set_signature(2, &SIG_2);

  lib7842::VisionReader reader(sensor);

  ObjDrawer drawer(parent);
  drawer.withStyle(LV_COLOR_HEX(0xFF7F00), LV_COLOR_WHITE);

  drawer.withLayer(reader)
  .withStyle(VISION_OBJECT_ERR_SIG, LV_COLOR_YELLOW, LV_COLOR_WHITE)
  .withStyle(1, LV_COLOR_BLUE, LV_COLOR_WHITE)
  .withStyle(2, LV_COLOR_RED, LV_COLOR_WHITE);

  while(true)
  {
    reader.reset();
    reader.getAll();
    reader.removeWith(objAttr::area, 0, 300);
    //reader.print();
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
