#include "OdomDisplay.hpp"

OdomDisplay::OdomDisplay(lv_obj_t* parent, lib7842::OdomTracker* tracker) : OdomDisplay(parent, lv_obj_get_style(parent)->body.main_color, tracker) {}

OdomDisplay::OdomDisplay(lv_obj_t* parent, lv_color_t mainColor, lib7842::OdomTracker* itracker) :
container(lv_obj_create(parent, NULL)), tracker(itracker), task(taskFnc, this)
{
  lv_obj_set_size(container, lv_obj_get_width(parent), lv_obj_get_height(parent));
  lv_obj_align(container, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_style_t* cStyle = new lv_style_t;
  lv_style_copy(cStyle, &lv_style_plain_color);
  cStyle->body.main_color = mainColor;
  cStyle->body.grad_color = mainColor;
  cStyle->body.border.width = 0;
  cStyle->body.radius = 0;
  lv_obj_set_style(container, cStyle);

  field = lv_obj_create(container, NULL);
  fieldDim = std::min(lv_obj_get_width(container), lv_obj_get_height(container));
  lv_obj_set_size(field, fieldDim, fieldDim);
  lv_obj_align(field, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
  lv_obj_set_style(field, cStyle);

  lv_style_t* grey = new lv_style_t;
  lv_style_t* red = new lv_style_t;
  lv_style_t* blue = new lv_style_t;
  lv_style_copy(grey, &lv_style_plain);
  grey->body.main_color = LV_COLOR_HEX(0x828F8F);
  grey->body.grad_color = LV_COLOR_HEX(0x828F8F);
  grey->body.border.width = 1;
  grey->body.radius = 0;
  grey->body.border.color = LV_COLOR_WHITE;
  lv_style_copy(red, grey);
  red->body.main_color = LV_COLOR_HEX(0xD42630);
  red->body.grad_color = LV_COLOR_HEX(0xD42630);
  lv_style_copy(blue, grey);
  blue->body.main_color = LV_COLOR_HEX(0x0077C9);
  blue->body.grad_color = LV_COLOR_HEX(0x0077C9);

  std::vector<std::vector<lv_style_t*>> data = {
    {grey, grey, grey, grey, grey, grey},
    {grey, grey, grey, grey, grey, grey},
    {red , grey, grey, grey, grey, blue},
    {grey, grey, grey, grey, grey, grey},
    {red , grey, grey, grey, grey, blue},
    {grey, grey, grey, grey, grey, grey}
  };

  double tileDim = fieldDim / data.size();

  for(int y = 0; y < data.size(); y++) {
    for(int x = 0; x < data[y].size(); x++) {
      lv_obj_t* tile = lv_obj_create(field, NULL);
      lv_obj_set_pos(tile, x * tileDim, y * tileDim);
      lv_obj_set_size(tile, tileDim, tileDim);
      lv_obj_set_style(tile, data[y][x]);
    }
  }


  led = lv_led_create(field, NULL);
  lv_led_on(led);
  lv_obj_set_size(led, tileDim / 3, tileDim / 3);

  lv_style_t* ledStyle = new lv_style_t;
  lv_style_copy(ledStyle, &lv_style_pretty_color);
  ledStyle->body.radius = LV_RADIUS_CIRCLE;
  ledStyle->body.main_color = LV_COLOR_YELLOW;
  ledStyle->body.grad_color = LV_COLOR_YELLOW;
  ledStyle->body.border.color = LV_COLOR_WHITE;
  ledStyle->body.border.width = 1;
  ledStyle->body.border.opa = LV_OPA_100;
  lv_obj_set_style(led, ledStyle);

}

OdomDisplay::~OdomDisplay() {
  lv_obj_del(container);
}


void OdomDisplay::drawRobot() {




}


void OdomDisplay::taskFnc(void* input)
{
  OdomDisplay* that = static_cast<OdomDisplay*>(input);

  while(true) {
    that->drawRobot();
    pros::delay(100);
  }
}
