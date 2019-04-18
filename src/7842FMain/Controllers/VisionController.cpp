#include "VisionController.hpp"
#include <sstream>
#include <iomanip>

using namespace lib7842;

VisionController::VisionController(pros::Vision* isensor, lv_obj_t* iparent, std::shared_ptr<okapi::SkidSteerModel> ichassis) :
sensor(isensor), reader(sensor), drawer(iparent),
chassis(ichassis),
task(taskFnc, this)
{
  //set up vision
  sensor->set_wifi_mode(0);
  sensor->set_exposure(30);
  pros::vision_signature_s_t SIG_1 = pros::Vision::signature_from_utility(1, -3189, -2595, -2892, 11723, 13047, 12384, 10.100, 0); sensor->set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 = pros::Vision::signature_from_utility(2, 8785, 9309, 9048, -585, -187, -386, 8.600, 0); sensor->set_signature(2, &SIG_2);

  drawer.setStyle(lv_obj_get_style(iparent)->body.main_color, LV_COLOR_WHITE);

  drawer.makeLayer(reader)
  .setStyle(1, LV_COLOR_BLUE, LV_COLOR_WHITE)
  .setStyle(2, LV_COLOR_RED, LV_COLOR_WHITE);
}

void VisionController::allign(lib7842::autonSides side) {

  if(side == lib7842::autonSides::red) {
    target = target.removeWithout(objAttr::sig, 2);
  } else {
    target = target.removeWithout(objAttr::sig, 1);
  }

  target.shrinkTo(3).sortBy(objAttr::middleY);

  lib7842::visionObj targetObj = target.get(0);

  double error = targetObj.centerX - VISION_FOV_WIDTH/2;

  double power;
  if(targetObj.sig == VISION_OBJECT_ERR_SIG) {
    power = 0;
  } else {
    power = error * 0.0014;
  }
  chassis->rotate(power);
}


void VisionController::run() {

  while(true)
  {
    reader.reset();
    reader.getAll();
    reader.removeWith(objAttr::area, 0, 200);
    reader.sortBy(objAttr::area);
    drawer.draw(reader);

    pros::delay(10);
  }
}


void VisionController::taskFnc(void* input) {
  pros::delay(500);
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
