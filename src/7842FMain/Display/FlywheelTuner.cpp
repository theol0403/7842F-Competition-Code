#include "FlywheelTuner.hpp"
#include <sstream>
#include <iomanip>

lv_color_t FlywheelTuner::needleColors[4] = {LV_COLOR_BLACK, LV_COLOR_BLUE, LV_COLOR_PURPLE, LV_COLOR_YELLOW};

FlywheelTuner::FlywheelTuner(lv_obj_t* parent) : FlywheelTuner(parent, lv_obj_get_style(parent)->body.main_color) {}

FlywheelTuner::FlywheelTuner(lv_obj_t* parent, lv_color_t imainColor) :
container(lv_obj_create(parent, NULL)), mainColor(imainColor),
task(taskFnc, this)
{
  /**
  * Container
  */
  lv_obj_set_size(container, lv_obj_get_width(parent), lv_obj_get_height(parent));
  lv_obj_align(container, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  lv_style_t* cStyle = new lv_style_t;
  lv_style_copy(cStyle, &lv_style_plain_color);
  cStyle->body.main_color = mainColor;
  cStyle->body.grad_color = mainColor;
  lv_obj_set_style(container, cStyle);
}

FlywheelTuner::~FlywheelTuner() {
  lv_obj_del(container);
}


FlywheelTuner &FlywheelTuner::withButton(std::string name, double* variable, btnType type, double modifier) {
  buttons.push_back(std::make_tuple(name, button_t{variable, type, modifier}, nullptr));
  return *this;
}

FlywheelTuner &FlywheelTuner::withGauge(std::string name, std::vector<double*> variables, int min, int max) {
  lv_obj_t* gauge = lv_gauge_create(container, NULL);
  lv_gauge_set_range(gauge, min, max);
  lv_gauge_set_critical_value(gauge, max);
  gauges.push_back(std::make_tuple(name, variables, gauge));
  return *this;
}

void FlywheelTuner::build() {

  std::vector<const char*>* btnLabels = new std::vector<const char*>;

  for(auto &button : buttons) {
    std::string* str = new std::string;
    *str = std::get<0>(button) + "+";
    btnLabels->push_back(str->c_str());
  }
  btnLabels->push_back("\n");
  for(auto &button : buttons) {
    std::string* str = new std::string;
    *str = std::get<0>(button) + "-";
    btnLabels->push_back(str->c_str());
  }

  btnLabels->push_back("");
  lv_obj_t* btnm = lv_btnm_create(container, NULL);
  lv_btnm_set_map(btnm, btnLabels->data());

  lv_obj_set_size(btnm, lv_obj_get_width(container), lv_obj_get_height(container) / 3);
  lv_obj_align(btnm, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_btnm_set_action(btnm, btnAction);
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

  for(auto &button : buttons) {
    lv_obj_t* label = lv_label_create(container, NULL);
    lv_obj_set_style(label, style_label);
    std::get<2>(button) = label;
  }
  calcLabels();

  lv_style_t* style_gauge = new lv_style_t;
  lv_style_copy(style_gauge, &lv_style_pretty_color);
  style_gauge->body.main_color = LV_COLOR_WHITE;     /*Line color at the beginning*/
  style_gauge->body.grad_color =  LV_COLOR_WHITE;    /*Line color at the end*/
  style_gauge->body.padding.hor = 10;                      /*Scale line length*/
  style_gauge->body.padding.inner = 8;                    /*Scale label padding*/
  style_gauge->body.border.color = LV_COLOR_HEX3(0x333);   /*Needle middle circle color*/
  style_gauge->line.width = 2;
  style_gauge->text.font = gauges.size() > 2 ? &lv_font_dejavu_10 : &lv_font_dejavu_20;
  style_gauge->text.letter_space = 1;
  style_gauge->text.color = LV_COLOR_WHITE;
  style_gauge->line.color = LV_COLOR_WHITE;                  /*Line color after the critical value*/

  lv_style_t* style_gauge_label = new lv_style_t;
  lv_style_copy(style_gauge_label, style_rel);
  style_gauge_label->text.font = gauges.size() > 4 ? &lv_font_dejavu_10 : &lv_font_dejavu_20;

  double gaugeSize = std::min((double)lv_obj_get_width(container) / gauges.size(), lv_obj_get_height(container) - lv_obj_get_height(container)/5.0);

  double offset = 0.0;
  for(auto &[name, variables, gauge] : gauges) {
    lv_obj_set_size(gauge, gaugeSize, gaugeSize);
    lv_gauge_set_needle_count(gauge, variables.size(), needleColors);
    lv_gauge_set_style(gauge, style_gauge);
    offset += (double)lv_obj_get_width(container)/gauges.size();
    lv_obj_align(gauge, NULL, LV_ALIGN_OUT_TOP_LEFT, offset - gaugeSize/2.0 - ((double)lv_obj_get_width(container)/gauges.size())/2.0, lv_obj_get_height(container)/3.0 + gaugeSize/2.0 + gaugeSize/6.0);

    lv_obj_t* label = lv_label_create(container, NULL);
    lv_label_set_text(label, name.c_str());
    lv_obj_set_style(label, style_gauge_label);
    lv_obj_align(label, NULL, LV_ALIGN_OUT_TOP_LEFT, offset - lv_obj_get_width(label)/2.0 - ((double)lv_obj_get_width(container)/gauges.size())/2.0, lv_obj_get_height(container)/3.0 + lv_obj_get_height(label)/2.0 + gaugeSize/3.0);
  }


}

void FlywheelTuner::calcLabels() {
  double offset = 0.0;
  for(auto &[name, button, label] : buttons) {
    std::stringstream str;
    int width = 1 + ((double)lv_obj_get_width(container)/buttons.size())/20; //this is very guessy, trying to account for decimal
    str << std::setprecision(width) << *button.variable;
    if(width > str.str().size()) width = str.str().size();
    str.str().erase(width, std::string::npos);
    lv_label_set_text(label, str.str().c_str());
    offset += (double)lv_obj_get_width(container)/buttons.size();
    lv_obj_align(label, NULL, LV_ALIGN_OUT_BOTTOM_LEFT, offset - lv_obj_get_width(label)/2.0 - (double)lv_obj_get_width(container)/buttons.size()/2.0, -lv_obj_get_height(container)/6.0 - lv_obj_get_height(label)/2.0);
  }
}

lv_res_t FlywheelTuner::btnAction(lv_obj_t* btnm, const char *itxt) {
  FlywheelTuner* that = static_cast<FlywheelTuner*>(lv_obj_get_free_ptr(btnm));
  std::string label = itxt;

  int labelPos = label.find_last_of({'+', '-'});
  bool sign = label.at(labelPos) == '+' ? true : false;
  label.erase(labelPos, std::string::npos);

  //find button in main vector from name
  auto search = std::find_if(that->buttons.begin(), that->buttons.end(), [=](const ButtonGroup &button){ return std::get<0>(button) == label; });
  if (search == that->buttons.end()) {
    std::cerr << "FlywheelTuner::btnAction : no label found" << std::endl;
  }

  button_t &button = std::get<1>(*search);

  switch(button.type)
  {
    case btnType::add: {
      if(sign) {
        *button.variable += that->multiplier;
      } else  {
        *button.variable -= that->multiplier;
      }
      if(*button.variable <= 0) *button.variable = 0.00000;
      break;
    }

    case btnType::multiply: {
      if(sign) {
        *button.variable *= button.modifier;
      } else {
        *button.variable /= button.modifier;
      }
      break;
    }

    case btnType::increment: {
      if(sign) {
        *button.variable += button.modifier;
      } else {
        *button.variable -= button.modifier;
      }
      break;
    }
  }

  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}


void FlywheelTuner::taskFnc(void* input) {
  pros::delay(500);
  FlywheelTuner* that = static_cast<FlywheelTuner*>(input);
  while(true) {
    that->calcLabels();

    for(auto &[name, variables, gauge] : that->gauges) {
      int i = 0;
      for(double* variable : variables) {
        lv_gauge_set_value(gauge, i, *variable);
        i++;
      }
    }
    pros::delay(100);
  }

}
