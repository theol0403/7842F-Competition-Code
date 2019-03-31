#include "AngleTuner.hpp"
#include <sstream>
#include <iomanip>

AngleTuner::AngleTuner(lv_obj_t* parent, ShootController*& iangler) : AngleTuner(parent, lv_obj_get_style(parent)->body.main_color, iangler) {}

AngleTuner::AngleTuner(lv_obj_t* parent, lv_color_t mainColor, ShootController*& iangler) :
container(parent), angler(iangler), task(taskFnc, this)
{

  std::vector<const char*>* btnLabels = new std::vector<const char*>;
  *btnLabels = {"Dist+", "\n", "Dist-", "\n", "Top+", "\n", "Top-", "\n", "Mid+", "\n", "Mid-", ""};

  lv_obj_t* btnm = lv_btnm_create(container, NULL);
  lv_btnm_set_map(btnm, btnLabels->data());

  lv_obj_set_size(btnm, lv_obj_get_width(container)/7, lv_obj_get_height(container));
  lv_obj_align(btnm, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
  lv_btnm_set_action(btnm, angleBtnAction);
  lv_obj_set_free_ptr(btnm, this);

  lv_style_t* style_bg = new lv_style_t;
  lv_style_copy(style_bg, &lv_style_plain);
  style_bg->body.main_color = mainColor;
  style_bg->body.grad_color = mainColor;
  style_bg->body.border.color = LV_COLOR_WHITE;
  style_bg->body.border.width = 3;
  style_bg->body.radius = 0;
  style_bg->body.padding.hor = 5;
  style_bg->body.padding.ver = 5;
  style_bg->body.padding.inner = 5;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BG, style_bg);

  lv_style_t* style_rel = new lv_style_t;
  lv_style_copy(style_rel, &lv_style_btn_tgl_rel);
  style_rel->body.main_color = mainColor;
  style_rel->body.grad_color = mainColor;
  style_rel->body.border.color = LV_COLOR_WHITE;
  style_rel->body.border.width = 2;
  style_rel->body.border.opa = LV_OPA_100;
  style_rel->body.radius = 2;
  style_rel->text.color = LV_COLOR_WHITE;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_REL, style_rel);

  lv_style_t* style_pr = new lv_style_t;
  lv_style_copy(style_pr, style_rel);
  style_pr->body.main_color = LV_COLOR_WHITE;
  style_pr->body.grad_color = LV_COLOR_WHITE;
  style_pr->text.color = mainColor;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_PR, style_pr);

  lv_style_t* style_ina = new lv_style_t;
  lv_style_copy(style_ina, &lv_style_btn_ina);
  style_ina->body.main_color = mainColor;
  style_ina->body.grad_color = mainColor;
  style_ina->body.border.width = 0;
  style_ina->text.color = LV_COLOR_WHITE;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_INA, style_ina);

  lv_style_t* style_label = new lv_style_t;
  lv_style_copy(style_label, &lv_style_plain);
  style_label->text.font = &lv_font_dejavu_20;
  style_label->text.letter_space = 1;
  style_label->text.color = LV_COLOR_WHITE;

  for(auto &label : angleLabels) {
    label = lv_label_create(container, NULL);
    lv_obj_set_style(label, style_label);
  }

  calcAngleLabels();
}

AngleTuner::~AngleTuner() {
  lv_obj_del(container);
}

void AngleTuner::allignLabel(lv_obj_t* label, std::string text, double* offset) {
  double height = lv_obj_get_height(container) - 5;
  lv_label_set_text(label, text.c_str());
  *offset += height/3.0;
  lv_obj_align(label, NULL, LV_ALIGN_OUT_TOP_LEFT, 75, *offset - lv_obj_get_height(label) + height/6.0);
}

void AngleTuner::calcAngleLabels() {
  double offset = -30.0;
  std::stringstream dist;
  dist << angler->distanceToFlag.convert(foot);
  allignLabel(angleLabels[0], dist.str(), &offset);
  std::stringstream topAngle;
  topAngle << angler->getTopFlagAngle();
  allignLabel(angleLabels[1], topAngle.str(), &offset);
  std::stringstream middleAngle;
  middleAngle << angler->getMiddleFlagAngle();
  allignLabel(angleLabels[2], middleAngle.str(), &offset);
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


void AngleTuner::taskFnc(void* input) {
  AngleTuner* that = static_cast<AngleTuner*>(input);
  while(true) {
    that->calcAngleLabels();

    pros::delay(50);
  }
}
