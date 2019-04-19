#include "VisionController.hpp"
#include <sstream>
#include <iomanip>
#include "7842FMain/RobotConfig.hpp"
#include "lib7842/odometry/controller.hpp"

using namespace lib7842;

VisionController::VisionController(pros::Vision* isensor, lv_obj_t* iparent, IterativePosPIDController* ipid) :
sensor(isensor), reader(sensor), drawer(iparent),
pid(ipid),
task(taskFnc, this)
{
  drawer.setStyle(lv_obj_get_style(iparent)->body.main_color, LV_COLOR_WHITE);
}

double VisionController::allign() {

  lib7842::visionObj targetObj = target.get(0);
  double error = targetObj.getAttr(objAttr::sig) == VISION_OBJECT_ERR_SIG ? 0 : targetObj.getAttr(objAttr::fromMidX);
  return pid->step(-error);

}


void VisionController::run() {

  //set up vision
  sensor->set_wifi_mode(0);
  sensor->set_exposure(150);
  pros::vision_signature_s_t SIG_1 = pros::Vision::signature_from_utility(1, -3681, -2409, -3045, 4469, 5685, 5077, 3.800, 1); sensor->set_signature(1, &SIG_1);
pros::vision_signature_s_t SIG_2 = pros::Vision::signature_from_utility(2, 11533, 12639, 12086, -1863, -1093, -1478, 6.800, 1); sensor->set_signature(2, &SIG_2);
pros::vision_signature_s_t SIG_3 = pros::Vision::signature_from_utility(3, -2149, -1107, -1628, -6621, -5863, -6242, 4.000, 1); sensor->set_signature(3, &SIG_3);

  drawer.makeLayer(reader)
  .setStyle(1, LV_COLOR_BLUE, LV_COLOR_WHITE)
  .setStyle(2, LV_COLOR_RED, LV_COLOR_WHITE)
  .setStyle(3, LV_COLOR_GREEN, LV_COLOR_WHITE)

  // drawer.makeLayer(target)
  // .setStyle(1, LV_COLOR_BLACK, LV_COLOR_BLUE)
  // .setStyle(2, LV_COLOR_BLACK, LV_COLOR_RED)
  // .setStyle(3, LV_COLOR_BLACK, LV_COLOR_GREEN);


  while(true)
  {
    //reading and simple filtering
    reader.reset();
    reader.getAll();
    //reader.removeWith(objAttr::isCode, false);

    // reader.removeWith(objAttr::area, 0, 200);
    // reader.sortBy(objAttr::area);
    // target = reader;
    //
    // //target finding
    // if(display.selector->getSelectedSide() == lib7842::autonSides::red) {
    //   target = target.removeWithout(objAttr::sig, 2);
    // } else {
    //   target = target.removeWithout(objAttr::sig, 1);
    // }
    //
    // //target.shrinkTo(3).sortBy(objAttr::absFromMidY);
    // target.shrinkTo(1);

    drawer.drawAll();
    reader.print();

    pros::delay(500);
  }
}


void VisionController::taskFnc(void* input) {
  pros::delay(500);
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
