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
  lv_obj_set_style(container, cStyle);

  field = lv_obj_create(container, NULL);
  fieldDim = std::min(lv_obj_get_width(container), lv_obj_get_height(container));
  lv_obj_set_size(field, fieldDim, fieldDim);
  lv_obj_align(field, NULL, LV_ALIGN_CENTER, 0, 0);

  lv_style_t* grey = new lv_style_t;
  lv_style_t* red = new lv_style_t;
  lv_style_t* blue = new lv_style_t;
  lv_style_copy(grey, &lv_style_plain);
  grey->body.main_color = LV_COLOR_HEX(0x999999);
  grey->body.grad_color = LV_COLOR_HEX(0x999999);
  grey->body.border.width = 1;
  grey->body.border.color = LV_COLOR_WHITE;
  lv_style_copy(red, grey);
  red->body.main_color = LV_COLOR_HEX(0xFF0000);
  red->body.grad_color = LV_COLOR_HEX(0xFF0000);
  lv_style_copy(blue, grey);
  blue->body.main_color = LV_COLOR_HEX(0x0000FF);
  blue->body.grad_color = LV_COLOR_HEX(0x0000FF);

  int tileCount = 6;
  lv_style_t* data[tileCount][tileCount] = {
    {grey, grey, grey, grey, grey, grey},
    {grey, grey, grey, grey, grey, grey},
    {red , grey, grey, grey, grey, blue},
    {grey, grey, grey, grey, grey, grey},
    {red , grey, grey, grey, grey, blue},
    {grey, grey, grey, grey, grey, grey}
  };

  double tileDim = fieldDim / tileCount;

  for(int y = 0; y < tileCount; y++) {
    for(int x = 0; x < tileCount; x++) {
      lv_obj_t* tile = lv_obj_create(field, NULL);
      lv_obj_set_pos(tile, x * tileDim, y * tileDim);
      lv_obj_set_size(tile, tileDim, tileDim);
      lv_obj_set_style(tile, data[y][x]);
    }
  }
}

OdomDisplay::~OdomDisplay() {
  lv_obj_del(container);
}


void OdomDisplay::run() {

  lv_point_t origPoints[] = { {0, 0}, {30, 0}, {24, -6}, {30, 0}, {24, 6}, {30, 0} };
  lv_point_t newPoints[6];

  lv_obj_t* arrow = lv_line_create(field, NULL);
  lv_obj_set_pos(arrow, 12, 108);
  lv_obj_set_style(arrow, &lv_style_plain);

  while(true) {

    lv_obj_set_pos(arrow, tracker->state.x.convert(foot)/12 * fieldDim, (12_ft - tracker->state.y).convert(foot)/12 * fieldDim);
    // rotateIt(origPoints, newPoints, 6, yeet.o);
    // fix(newPoints, 6);
    lv_line_set_points(arrow, origPoints, 6);
    lv_obj_invalidate(arrow);

    pros::delay(100);
  }
}

void OdomDisplay::taskFnc(void* input)
{
  OdomDisplay* that = static_cast<OdomDisplay*>(input);
  that->run();
}
