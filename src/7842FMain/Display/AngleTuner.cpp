#include "AngleTuner.hpp"
#include <sstream>
#include <iomanip>

AngleTuner::AngleTuner(lv_obj_t* parent, ShootController*& iangler) : AngleTuner(parent, lv_obj_get_style(parent)->body.main_color, iangler) {}

AngleTuner::AngleTuner(lv_obj_t* parent, lv_color_t mainColor, ShootController*& iangler) :
container(parent), angler(iangler), task(taskFnc, this)
{

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
  lv_obj_t* angleContainer = lv_obj_create(container, NULL);
  lv_obj_set_size(angleContainer, lv_obj_get_width(container)/5, lv_obj_get_height(container));
  lv_obj_align(angleContainer, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
  lv_obj_set_style(angleContainer, masterStyle);

  std::vector<const char*>* btnLabels = new std::vector<const char*>;
  *btnLabels = {"Dist+", "\n", "Dist-", "\n", "Top+", "\n", "Top-", "\n", "Mid+", "\n", "Mid-", ""};

  lv_obj_t* angleBtnm = lv_btnm_create(angleContainer, NULL);
  lv_btnm_set_map(angleBtnm, btnLabels->data());

  lv_obj_set_size(angleBtnm, lv_obj_get_width(angleContainer)/3*2, lv_obj_get_height(angleContainer));
  lv_obj_align(angleBtnm, NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);
  lv_btnm_set_action(angleBtnm, angleBtnAction);
  lv_obj_set_free_ptr(angleBtnm, this);

  lv_btnm_set_style(angleBtnm, LV_BTNM_STYLE_BG, btnm_bg);
  lv_btnm_set_style(angleBtnm, LV_BTNM_STYLE_BTN_REL, btnm_rel);
  lv_btnm_set_style(angleBtnm, LV_BTNM_STYLE_BTN_PR, btnm_pr);
  lv_btnm_set_style(angleBtnm, LV_BTNM_STYLE_BTN_INA, btnm_ina);

  lv_style_t* style_label = new lv_style_t;
  lv_style_copy(style_label, &lv_style_plain);
  style_label->text.font = &lv_font_dejavu_20;
  style_label->text.letter_space = 1;
  style_label->text.color = LV_COLOR_WHITE;

  for(auto &label : angleLabels) {
    label = lv_label_create(angleContainer, NULL);
    lv_obj_set_style(label, style_label);
  }

  calcAngleLabels();
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

    pros::delay(100);
  }
}
