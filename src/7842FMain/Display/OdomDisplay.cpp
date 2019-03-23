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

  lv_style_t* fStyle = new lv_style_t;
  lv_style_copy(fStyle, cStyle);
  fStyle->body.main_color = LV_COLOR_WHITE;
  fStyle->body.grad_color = LV_COLOR_WHITE;
  lv_obj_set_style(field, fStyle);

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

  for(double y = 0; y < data.size(); y++) {
    for(double x = 0; x < data[y].size(); x++) {
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

  lv_color_t mainColor = lv_obj_get_style(container)->body.main_color;

  lv_obj_t* led = lv_led_create(field, NULL);
  lv_led_on(led);
  lv_obj_set_size(led, fieldDim / 15, fieldDim / 15);

  lv_style_t ledStyle;
  lv_style_copy(&ledStyle, &lv_style_plain);
  ledStyle.body.radius = LV_RADIUS_CIRCLE;
  ledStyle.body.main_color = mainColor;
  ledStyle.body.grad_color = mainColor;
  ledStyle.body.border.color = LV_COLOR_WHITE;
  ledStyle.body.border.width = 2;
  ledStyle.body.border.opa = LV_OPA_100;
  ledStyle.text.color = LV_COLOR_WHITE;
  ledStyle.text.opa = LV_OPA_100;
  lv_obj_set_style(led, &ledStyle);

  std::vector<lv_point_t> points = {{0, 0}, {0, 0}};

  lv_obj_t* arrow = lv_line_create(field, NULL);
  lv_line_set_points(arrow, points.data(), points.size());
  lv_obj_set_pos(arrow, 0, 0);

  lv_style_t arrowStyle;
  lv_style_copy(&arrowStyle, &lv_style_plain);
  const int lineWidth = 2;
  arrowStyle.line.width = lineWidth;
  arrowStyle.line.opa = LV_OPA_100;
  arrowStyle.line.color = mainColor;
  lv_obj_set_style(arrow, &arrowStyle);

  int arrowHeight = fieldDim / 6;

  lv_obj_t* label = lv_label_create(container, NULL);
  lv_obj_set_style(label, &ledStyle);
  std::string text;

  text =
  "X: " + std::to_string(tracker->getX().convert(foot)) + "\n" +
  "Y: " + std::to_string(tracker->getY().convert(foot)) + "\n" +
  "Theta: " + std::to_string(tracker->getTheta().convert(degree));
  lv_label_set_text(label, text.c_str());
  lv_obj_align(label, container, LV_ALIGN_CENTER, -lv_obj_get_width(container)/2 + (lv_obj_get_width(container) - fieldDim)/2, 0);

  while(true) {

    double x = tracker->getX().convert(court);
    double y = (1_crt - tracker->getY()).convert(court);
    double theta = tracker->getTheta().convert(radian);

    lv_obj_set_pos(led, (x * fieldDim) - lv_obj_get_width(led)/2, (y * fieldDim) - lv_obj_get_height(led)/2 - 1);

    points[0] = {(short)((x * fieldDim)), (short)((y * fieldDim) - (lineWidth/2))};
    double newY = arrowHeight * cos(theta);
    double newX = arrowHeight * sin(theta);
    points[1] = {(short)(newX + points[0].x), (short)(-newY + points[0].y)};

    lv_line_set_points(arrow, points.data(), points.size());
    lv_obj_invalidate(arrow);

    text =
    "X: " + std::to_string(tracker->getX().convert(foot)) + "\n" +
    "Y: " + std::to_string(tracker->getY().convert(foot)) + "\n" +
    "Theta: " + std::to_string(tracker->getTheta().convert(degree));
    lv_label_set_text(label, text.c_str());

    pros::delay(50);
  }

}


void OdomDisplay::taskFnc(void* input)
{
  OdomDisplay* that = static_cast<OdomDisplay*>(input);
  that->run();
}
