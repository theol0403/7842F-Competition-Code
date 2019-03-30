#include "AngleTuner.hpp"
#include <sstream>
#include <iomanip>

AngleTuner::AngleTuner(lv_obj_t* parent) : AngleTuner(parent, lv_obj_get_style(parent)->body.main_color) {}

AngleTuner::AngleTuner(lv_obj_t* parent, lv_color_t imainColor) :
container(parent), task(taskFnc, this)
{

  std::vector<const char*> btnLabels;
  btnLabels = {"Dist+", "Dist-", "\n", "Top+", "Top-", "\n", "Mid+", "Mid-", ""};

  lv_obj_t* btnm = lv_btnm_create(container, NULL);
  lv_btnm_set_map(btnm, btnLabels->data());

  lv_obj_set_size(btnm, lv_obj_get_width(container)/3, lv_obj_get_height(container));
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
  style_label->text.color = LV_COLOR_BLACK;

  // for(auto &button : buttons) {
  //   lv_obj_t* label = lv_label_create(container, NULL);
  //   lv_obj_set_style(label, style_label);
  //   std::get<2>(button) = label;
  // }
  // calcLabels();





}

AngleTuner::~AngleTuner() {
  lv_obj_del(container);
}


// void AngleTuner::build() {
//
//   lv_style_t* style_gauge = new lv_style_t;
//   lv_style_copy(style_gauge, &lv_style_pretty_color);
//   style_gauge->body.main_color = LV_COLOR_WHITE;     /*Line color at the beginning*/
//   style_gauge->body.grad_color =  LV_COLOR_WHITE;    /*Line color at the end*/
//   style_gauge->body.padding.hor = 10;                      /*Scale line length*/
//   style_gauge->body.padding.inner = 8;                    /*Scale label padding*/
//   style_gauge->body.border.color = LV_COLOR_HEX3(0x333);   /*Needle middle circle color*/
//   style_gauge->line.width = 2;
//   style_gauge->text.font = gauges.size() > 2 ? &lv_font_dejavu_10 : &lv_font_dejavu_20;
//   style_gauge->text.letter_space = 1;
//   style_gauge->text.color = LV_COLOR_WHITE;
//   style_gauge->line.color = LV_COLOR_WHITE;                  /*Line color after the critical value*/
//
//   lv_style_t* style_gauge_label = new lv_style_t;
//   lv_style_copy(style_gauge_label, style_rel);
//   style_gauge_label->text.font = gauges.size() > 4 ? &lv_font_dejavu_10 : &lv_font_dejavu_20;
//
//   double gaugeSize = std::min((double)lv_obj_get_width(container) / gauges.size(), lv_obj_get_height(container) - lv_obj_get_height(container)/5.0);
//
//   double offset = 0.0;
//   for(auto &[name, variables, gauge] : gauges) {
//     lv_obj_set_size(gauge, gaugeSize, gaugeSize);
//     lv_gauge_set_needle_count(gauge, variables.size(), needleColors);
//     lv_gauge_set_style(gauge, style_gauge);
//     offset += (double)lv_obj_get_width(container)/gauges.size();
//     lv_obj_align(gauge, NULL, LV_ALIGN_OUT_TOP_LEFT, offset - gaugeSize/2.0 - ((double)lv_obj_get_width(container)/gauges.size())/2.0, lv_obj_get_height(container)/3.0 + gaugeSize/2.0 + gaugeSize/6.0);
//
//     lv_obj_t* label = lv_label_create(container, NULL);
//     lv_label_set_text(label, name.c_str());
//     lv_obj_set_style(label, style_gauge_label);
//     lv_obj_align(label, NULL, LV_ALIGN_OUT_TOP_LEFT, offset - lv_obj_get_width(label)/2.0 - ((double)lv_obj_get_width(container)/gauges.size())/2.0, lv_obj_get_height(container)/3.0 + lv_obj_get_height(label)/2.0 + gaugeSize/3.0);
//   }
//
//
// }

lv_res_t AngleTuner::angleBtnAction(lv_obj_t* btnm, const char *itxt) {
  // AngleTuner* that = static_cast<AngleTuner*>(lv_obj_get_free_ptr(btnm));
  // std::string label = itxt;
  //
  // int labelPos = label.find_last_of({'+', '-'});
  // bool sign = label.at(labelPos) == '+' ? true : false;
  // label.erase(labelPos, std::string::npos);
  //
  // //find button in main vector from name
  // auto search = std::find_if(that->buttons.begin(), that->buttons.end(), [=](const ButtonGroup &button){ return std::get<0>(button) == label; });
  // if (search == that->buttons.end()) {
  //   std::cerr << "AngleTuner::btnAction : no label found" << std::endl;
  // }
  //
  // button_t &button = std::get<1>(*search);
  //
  // switch(button.type)
  // {
  //   case btnType::add: {
  //     if(sign) {
  //       *button.variable += that->multiplier;
  //     } else  {
  //       *button.variable -= that->multiplier;
  //     }
  //     if(*button.variable <= 0) *button.variable = 0.00000;
  //     break;
  //   }
  //
  //   case btnType::multiply: {
  //     if(sign) {
  //       *button.variable *= button.modifier;
  //     } else {
  //       *button.variable /= button.modifier;
  //     }
  //     break;
  //   }
  //
  //   case btnType::increment: {
  //     if(sign) {
  //       *button.variable += button.modifier;
  //     } else {
  //       *button.variable -= button.modifier;
  //     }
  //     break;
  //   }
  // }

  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}


void AngleTuner::taskFnc(void* input) {
  // AngleTuner* that = static_cast<AngleTuner*>(input);
  // while(true) {
  //   that->calcLabels();
  //
  //   for(auto &[name, variables, gauge] : that->gauges) {
  //     int i = 0;
  //     for(double* variable : variables) {
  //       lv_gauge_set_value(gauge, i, *variable);
  //       i++;
  //     }
  //   }
  //   pros::delay(50);
  // }

}
