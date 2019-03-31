#include "AngleTuner.hpp"
#include <sstream>
#include <iomanip>

AngleTuner::AngleTuner(lv_obj_t* parent, ShootController*& iangler) : AngleTuner(parent, lv_obj_get_style(parent)->body.main_color, iangler) {}

AngleTuner::AngleTuner(lv_obj_t* parent, lv_color_t mainColor, ShootController*& iangler) :
container(parent), angler(iangler), task(taskFnc, this)
{

  int angleContainerWidth = lv_obj_get_width(container)/5.0;
  int angleContainerHeight = lv_obj_get_height(container);

  int actionContainerWidth = lv_obj_get_width(container) - angleContainerWidth;
  int actionContainerHeight = lv_obj_get_height(container)/5.0;

  int dialContainerWidth = (lv_obj_get_width(container) - angleContainerWidth) / 3;
  int dialContainerHeight = lv_obj_get_height(container) - actionContainerHeight;

  /**
  * ButtonMatrix Styles
  */
  lv_style_t* btnm_bg = new lv_style_t;
  lv_style_copy(btnm_bg, &lv_style_plain);
  btnm_bg->body.main_color = mainColor;
  btnm_bg->body.grad_color = mainColor;
  btnm_bg->body.border.color = LV_COLOR_WHITE;
  btnm_bg->body.border.width = 0;
  btnm_bg->body.radius = 0;
  btnm_bg->body.padding.hor = 5;
  btnm_bg->body.padding.ver = 5;
  btnm_bg->body.padding.inner = 5;

  lv_style_t* btnm_rel = new lv_style_t;
  lv_style_copy(btnm_rel, &lv_style_btn_tgl_rel);
  btnm_rel->body.main_color = mainColor;
  btnm_rel->body.grad_color = mainColor;
  btnm_rel->body.border.color = LV_COLOR_WHITE;
  btnm_rel->body.border.width = 2;
  btnm_rel->body.border.opa = LV_OPA_100;
  btnm_rel->body.radius = 2;
  btnm_rel->text.color = LV_COLOR_WHITE;

  lv_style_t* btnm_pr = new lv_style_t;
  lv_style_copy(btnm_pr, btnm_rel);
  btnm_pr->body.main_color = LV_COLOR_WHITE;
  btnm_pr->body.grad_color = LV_COLOR_WHITE;
  btnm_pr->text.color = mainColor;

  lv_style_t* btnm_ina = new lv_style_t;
  lv_style_copy(btnm_ina, &lv_style_btn_ina);
  btnm_ina->body.main_color = mainColor;
  btnm_ina->body.grad_color = mainColor;
  btnm_ina->body.border.width = 0;
  btnm_ina->text.color = LV_COLOR_WHITE;

  /**
  * Label Style
  */
  lv_style_t* style_label = new lv_style_t;
  lv_style_copy(style_label, &lv_style_plain);
  style_label->text.font = &lv_font_dejavu_20;
  style_label->text.letter_space = 1;
  style_label->text.color = LV_COLOR_WHITE;

  /**
  * Container Master Style
  */
  lv_style_t* masterStyle = new lv_style_t;
  lv_style_copy(masterStyle, &lv_style_plain);
  masterStyle->body.main_color = mainColor;
  masterStyle->body.grad_color = mainColor;
  masterStyle->body.border.color = LV_COLOR_WHITE;
  masterStyle->body.border.width = 3;
  masterStyle->body.radius = 0;

  /**
  * Angle Buttons
  */
  {
    lv_obj_t* angleContainer = lv_obj_create(container, NULL);
    lv_obj_set_size(angleContainer, angleContainerWidth, angleContainerHeight);
    lv_obj_align(angleContainer, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_set_style(angleContainer, masterStyle);

    std::vector<const char*>* btnLabels = new std::vector<const char*>;
    *btnLabels = {"Dist+", "\n", "Dist-", "\n", "Top+", "\n", "Top-", "\n", "Mid+", "\n", "Mid-", ""};

    lv_obj_t* btnm = lv_btnm_create(angleContainer, NULL);
    lv_btnm_set_map(btnm, btnLabels->data());

    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BG, btnm_bg);
    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_REL, btnm_rel);
    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_PR, btnm_pr);
    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_INA, btnm_ina);

    lv_obj_set_size(btnm, lv_obj_get_width(angleContainer)/3*2, lv_obj_get_height(angleContainer));
    lv_btnm_set_action(btnm, angleBtnAction);
    lv_obj_set_free_ptr(btnm, this);

    /**
    * Angle Labels
    */
    for(auto &label : angleLabels) {
      label = lv_label_create(angleContainer, NULL);
      lv_obj_set_style(label, style_label);
    }
    calcAngleLabels();
  }


  /**
  * Action Buttons
  */
  {
    lv_obj_t* actionContainer = lv_obj_create(container, NULL);
    lv_obj_set_size(actionContainer, actionContainerWidth, actionContainerHeight);
    lv_obj_align(actionContainer, container, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    lv_obj_set_style(actionContainer, masterStyle);

    std::vector<const char*>* actionLabels = new std::vector<const char*>;
    *actionLabels = {"Top", "Mid", "Cycle", "Shoot", ""};

    lv_obj_t* btnm = lv_btnm_create(actionContainer, NULL);
    lv_btnm_set_map(btnm, actionLabels->data());

    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BG, btnm_bg);
    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_REL, btnm_rel);
    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_PR, btnm_pr);
    lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_INA, btnm_ina);

    lv_obj_set_size(btnm, lv_obj_get_width(actionContainer), lv_obj_get_height(actionContainer));
    lv_btnm_set_action(btnm, actionBtnAction);
    lv_obj_set_free_ptr(btnm, this);
  }


  /**
  * Angle Dial
  */
  {
    lv_obj_t* dialContainer = lv_obj_create(container, NULL);
    lv_obj_set_size(dialContainer, dialContainerWidth, dialContainerHeight);
    lv_obj_align(dialContainer, container, LV_ALIGN_IN_BOTTOM_LEFT, angleContainerWidth, 0);
    lv_obj_set_style(dialContainer, masterStyle);
    lv_obj_t* led = lv_led_create(dialContainer, NULL);
    lv_led_on(led);
    lv_obj_set_size(led, 10, 10);

    lv_style_t* ledStyle = new lv_style_t;
    lv_style_copy(ledStyle, &lv_style_plain);
    ledStyle->body.radius = LV_RADIUS_CIRCLE;
    ledStyle->body.main_color = mainColor;
    ledStyle->body.grad_color = mainColor;
    ledStyle->body.border.color = LV_COLOR_WHITE;
    ledStyle->body.border.width = 2;
    ledStyle->body.border.opa = LV_OPA_100;
    lv_obj_set_style(led, ledStyle);
    lv_obj_align(led, NULL, LV_ALIGN_CENTER, 0, 0);

    /**
    * Angle Dial Line
    */
    lv_style_t lineStyle;
    lv_style_copy(&lineStyle, &lv_style_plain);
    lineStyle.line.width = 2;
    lineStyle.line.opa = LV_OPA_100;
    lineStyle.line.color = LV_COLOR_WHITE;

    std::vector<lv_color_t> lineColors = {LV_COLOR_WHITE, LV_COLOR_RED, LV_COLOR_BLUE};
    for(int i = 0; i < dialLines.size(); i++) {
      auto &[points, line] = dialLines[i];
      points[0] = {0, 0};
      points[1] = {0, 0};
      line = lv_line_create(dialContainer, NULL);
      lv_style_t* style = new lv_style_t;
      lv_style_copy(style, &lineStyle);
      style->line.color = lineColors[i];
      lv_obj_set_style(line, style);

      lv_line_set_points(line, points.data(), points.size());
      lv_obj_align(line, NULL, LV_ALIGN_CENTER, 0, 0);
    }

    /**
    * Angle Dial Label
    */

  }




}

AngleTuner::~AngleTuner() {
  lv_obj_del(container);
}

void AngleTuner::allignLabel(lv_obj_t* label, std::string text, double* offset) {
  int height = lv_obj_get_height(container) - 15;
  lv_label_set_text(label, text.c_str());
  *offset += height/3.0;
  lv_obj_align(label, NULL, LV_ALIGN_OUT_TOP_LEFT, 65, *offset - lv_obj_get_height(label) + height/6.0);
}

void AngleTuner::calcAngleLabels() {
  double offset = -30.0;
  {
    std::stringstream str;
    str << angler->distanceToFlag.convert(foot);
    allignLabel(angleLabels[0], str.str(), &offset);
  }
  {
    std::stringstream str;
    str << angler->getTopFlagAngle();
    allignLabel(angleLabels[1], str.str(), &offset);
  }
  {
    std::stringstream str;
    str << angler->getMiddleFlagAngle();
    allignLabel(angleLabels[2], str.str(), &offset);
  }
}


lv_res_t AngleTuner::angleBtnAction(lv_obj_t* btnm, const char *itxt) {
  AngleTuner* that = static_cast<AngleTuner*>(lv_obj_get_free_ptr(btnm));
  std::string label = itxt;

  int labelPos = label.find_last_of({'+', '-'});
  bool sign = label.at(labelPos) == '+' ? true : false;
  label.erase(labelPos, std::string::npos);

  if(label == "Dist") {
    that->angler->distanceToFlag += 0.5_ft * boolToSign(sign);
  } else if(label == "Top") {
    that->angler->topAngles.at(that->angler->distanceToFlag.convert(foot)) += 0.5 * boolToSign(sign);
  } else if(label == "Mid") {
    that->angler->middleAngles.at(that->angler->distanceToFlag.convert(foot)) += 0.5 * boolToSign(sign);
  } else {
    std::cerr << "No Button Found" << std::endl;
  }

  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}


lv_res_t AngleTuner::actionBtnAction(lv_obj_t* btnm, const char *itxt) {
  AngleTuner* that = static_cast<AngleTuner*>(lv_obj_get_free_ptr(btnm));
  std::string label = itxt;

  if(label == "Top") {
    that->angler->doJobLoop(ShootController::angleTop);
  } else if(label == "Mid") {
    that->angler->doJobLoop(ShootController::angleMiddle);
  } else if(label == "Cycle") {
    that->angler->doJob(ShootController::cycle);
  } else if(label == "Shoot") {
    that->angler->doJob(ShootController::enableShoot);
  } else {
    std::cerr << "No Button Found" << std::endl;
  }
  std::cout << label << std::endl;

  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}


void AngleTuner::calcDial() {
  int lineLength = 30;

  {
    auto &[points, line] = dialLines[0];
    points[1] = {(short)(lineLength * cos(45)), (short)(lineLength * sin(45))};
    lv_line_set_points(line, points.data(), points.size());
    lv_obj_invalidate(line);
  }

  // {
  //   auto &[points, line] = dialLines[1];
  //   points[1] = {(short)(lineLength * cos(10)), (short)(lineLength * sin(10))};
  //   lv_line_set_points(line, points.data(), points.size());
  //   lv_obj_invalidate(line);
  // }
  //
  // {
  //   auto &[points, line] = dialLines[2];
  //   points[1] = {(short)(lineLength * cos(90)), (short)(lineLength * sin(90))};
  //   lv_line_set_points(line, points.data(), points.size());
  //   lv_obj_invalidate(line);
  // }

}


void AngleTuner::taskFnc(void* input) {
  pros::delay(500);
  AngleTuner* that = static_cast<AngleTuner*>(input);
  while(true) {
    that->calcAngleLabels();
    that->calcDial();

    pros::delay(100);
  }
}
