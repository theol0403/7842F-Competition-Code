#include "VisionController.hpp"
#include "lib7842/vision/visionReader.hpp"
#include "lib7842/vision/objDrawer.hpp"
#include <sstream>
#include <iomanip>

using namespace lib7842;

VisionController::VisionController(pros::Vision* isensor, lv_obj_t* iparent) :
sensor(isensor), parent(iparent),
task(taskFnc, this)
{
}


void VisionController::run()
{

  sensor->set_exposure(150);
  pros::vision_signature_s_t SIG_1 = {1, {1, 0, 0}, 2.800, -2129, -1369, -1749, 3221, 4401, 3811, 0, 0}; sensor->set_signature(1, &SIG_1);
  pros::vision_signature_s_t SIG_2 = {2, {1, 0, 0}, 7.600, 6823, 7385, 7104, -2009, -1543, -1776, 0, 0}; sensor->set_signature(2, &SIG_2);

  lib7842::VisionReader reader(sensor);
  lib7842::ObjContainer standout;

  ObjDrawer drawer(parent);
  drawer.withStyle(lv_obj_get_style(parent)->body.main_color, LV_COLOR_WHITE);

  drawer.withLayer(reader)
  .withStyle(VISION_OBJECT_ERR_SIG, LV_COLOR_YELLOW, LV_COLOR_WHITE)
  .withStyle(1, LV_COLOR_BLUE, LV_COLOR_WHITE)
  .withStyle(2, LV_COLOR_RED, LV_COLOR_WHITE);

  lv_obj_t* infoLabel = lv_label_create(parent, NULL);
  std::string infoText;
  lv_style_t textStyle;
  lv_style_copy(&textStyle, &lv_style_plain);
  textStyle.text.color = LV_COLOR_WHITE;
  textStyle.text.opa = LV_OPA_100;
  lv_obj_set_style(infoLabel, &textStyle);

  while(true)
  {
    reader.reset();
    reader.getAll();
    reader.removeWith(objAttr::area, 0, 300);
    reader.sortBy(objAttr::area);
    drawer.draw();

    infoText = "Count: " + std::to_string(reader.getCount());
    lv_label_set_text(infoLabel, infoText.c_str());
    lv_obj_align(infoLabel, parent, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    pros::delay(50);
  }
}


void VisionController::taskFnc(void* input)
{
  pros::delay(500);
  VisionController* that = static_cast<VisionController*>(input);
  that->run();
}
