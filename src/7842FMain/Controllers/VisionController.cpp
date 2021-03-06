#include "VisionController.hpp"
#include "7842FMain/RobotConfig.hpp"
#include "lib7842/odometry/controller.hpp"
#include <iomanip>
#include <sstream>

using namespace lib7842;

VisionController::VisionController(pros::Vision* isensor, lv_obj_t* iparent,
                                   IterativePosPIDController* ipid) :
  sensor(isensor), reader(sensor), drawer(iparent), pid(ipid), task(taskFnc, this) {
  drawer.setStyle(lv_obj_get_style(iparent)->body.main_color, LV_COLOR_WHITE);
}

double VisionController::allign() {

  lib7842::visionObj targetObj = target.get(0);
  double error = targetObj.getAttr(objAttr::sig) == VISION_OBJECT_ERR_SIG
                   ? 0
                   : targetObj.getAttr(objAttr::fromMidX);
  return pid->step(-error);
}

void VisionController::run() {

  // set up vision
  sensor->set_wifi_mode(0);
  sensor->set_exposure(40);
  pros::vision_signature_s_t SIG_1 =
    pros::Vision::signature_from_utility(1, -3247, -3003, -3124, 13677, 14209, 13944, 11.000, 0);
  sensor->set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 =
    pros::Vision::signature_from_utility(2, 7733, 8299, 8016, 205, 825, 516, 10.500, 0);
  sensor->set_signature(2, &SIG_2);

  drawer.makeLayer(reader)
    .setStyle(1, LV_COLOR_BLUE, LV_COLOR_WHITE)
    .setStyle(2, LV_COLOR_RED, LV_COLOR_WHITE)
    .setStyle(3, LV_COLOR_GREEN, LV_COLOR_WHITE);

  drawer.makeLayer(target)
    .setStyle(1, LV_COLOR_BLACK, LV_COLOR_BLUE)
    .setStyle(2, LV_COLOR_BLACK, LV_COLOR_RED)
    .setStyle(3, LV_COLOR_BLACK, LV_COLOR_GREEN);

  while (true) {
    // reading and simple filtering
    reader.reset();
    reader.getAll();

    reader.removeWith(objAttr::area, 0, 200).sortBy(objAttr::area);
    target = reader;

    // target finding
    if (display.selector->getSelectedSide() == lib7842::autonSides::red) {
      target = target.removeWithout(objAttr::sig, 1);
    } else {
      target = target.removeWithout(objAttr::sig, 2);
    }

    target.shrinkTo(3);
    target.snapToGridY(8).sortBy(objAttr::absFromMidY, false);
    target.shrinkTo(1);

    drawer.drawAll();

    pros::delay(10);
  }
}

void VisionController::taskFnc(void* input) {
  pros::delay(500);
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
