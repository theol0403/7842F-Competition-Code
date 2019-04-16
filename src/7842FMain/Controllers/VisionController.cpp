#include "VisionController.hpp"
#include "lib7842/vision/visionReader.hpp"
#include "lib7842/vision/objDrawer.hpp"
#include <sstream>
#include <iomanip>

using namespace lib7842;

VisionController::VisionController(pros::Vision* isensor, lv_obj_t* iparent, std::shared_ptr<okapi::SkidSteerModel> ichassis) :
sensor(isensor), parent(iparent), chassis(ichassis),
task(taskFnc, this)
{
}

void VisionController::allign(lib7842::autonSides side) {

  lib7842::visionObj targetObj;
  if(side == lib7842::autonSides::red) {
    targetObj = target.removeWithout(objAttr::sig, 1).get(0);
  } else {
    targetObj = target.removeWithout(objAttr::sig, 2).get(0);
  }

  double error = targetObj.centerX - VISION_FOV_WIDTH/2;

  double power;
  if(targetObj.sig == VISION_OBJECT_ERR_SIG) {
    power = 0;
  } else {
    power = error * 0.0014;
  }
  chassis->rotate(power);
}


void VisionController::run()
{

  sensor->set_wifi_mode(0);
  sensor->set_exposure(30);
  pros::vision_signature_s_t SIG_1 = pros::Vision::signature_from_utility(1, 8785, 9309, 9048, -585, -187, -386, 8.600, 0); sensor->set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 = pros::Vision::signature_from_utility(2, -3189, -2595, -2892, 11723, 13047, 12384, 10.100, 0); sensor->set_signature(2, &SIG_2);

  lib7842::VisionReader reader(sensor);

  ObjDrawer drawer(parent);
  drawer.withStyle(lv_obj_get_style(parent)->body.main_color, LV_COLOR_WHITE);

  drawer.withLayer(reader)
  .withStyle(1, LV_COLOR_BLUE, LV_COLOR_WHITE)
  .withStyle(2, LV_COLOR_RED, LV_COLOR_WHITE);

  while(true)
  {
    reader.reset();
    reader.getAll();
    //reader.removeWith(objAttr::area, 0, 400).removeWith(objAttr::y, 0, 50);
    reader.sortBy(objAttr::area);
    target = reader;
    drawer.draw();

    pros::delay(50);
  }
}


void VisionController::taskFnc(void* input) {
  pros::delay(500);
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
