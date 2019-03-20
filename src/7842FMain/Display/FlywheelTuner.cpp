#include "FlywheelTuner.hpp"


FlywheelTuner::FlywheelTuner(lv_obj_t* parent) :
container(lv_obj_create(parent, NULL)), mainColor(LV_COLOR_HEX(0xFF7F00))
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
  lv_obj_set_style(container, cStyle);

}


FlywheelTuner &FlywheelTuner::withButton(std::string name, double* variable, buttonType_t type, double modifier) {
  buttons.insert_or_assign(name, button_t{variable, type, modifier});
  return *this;
}

void FlywheelTuner::build() {

  std::vector<std::string>* snames = new std::vector<std::string>;
  for(auto button : buttons) {
    snames->push_back(button.first);
  }

  std::vector<const char*> cnames;
  for(std::string &sname : *snames) {
    cnames.push_back(sname.c_str());
    std::cout << sname << std::endl;
  }
  cnames.push_back("");

  lv_obj_t* btnm = lv_btnm_create(container, NULL);
  lv_btnm_set_map(btnm, cnames.data());
  lv_obj_set_size(btnm, lv_obj_get_width(container), lv_obj_get_height(container) / 3);
  lv_obj_align(container, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  // lv_btnm_set_action(btnm, matrixAction);
  // lv_obj_set_free_ptr(btnm, this);

  lv_style_t* style_bg = new lv_style_t;
  lv_style_copy(style_bg, &lv_style_plain);
  style_bg->body.main_color = mainColor;
  style_bg->body.grad_color = mainColor;
  style_bg->body.border.color = LV_COLOR_WHITE;
  style_bg->body.border.width = 3;
  style_bg->body.radius = 0;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BG, style_bg);

  lv_style_t* style_rel = new lv_style_t;
  lv_style_copy(style_rel, &lv_style_btn_tgl_rel);
  style_rel->body.main_color = mainColor;
  style_rel->body.grad_color = mainColor;
  style_rel->body.border.color = LV_COLOR_WHITE;
  style_rel->body.border.width = 3;
  style_rel->body.border.opa = LV_OPA_100;
  style_rel->body.radius = 10;
  style_rel->text.color = LV_COLOR_WHITE;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_TGL_REL, style_rel);

  lv_style_t* style_pr = new lv_style_t;
  lv_style_copy(style_pr, style_rel);
  style_pr->body.main_color = LV_COLOR_WHITE;
  style_pr->body.grad_color = LV_COLOR_WHITE;
  style_pr->text.color = mainColor;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_TGL_PR, style_pr);
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_PR, style_pr);

  lv_style_t* style_ina = new lv_style_t;
  lv_style_copy(style_ina, &lv_style_btn_ina);
  style_ina->body.main_color = mainColor;
  style_ina->body.grad_color = mainColor;
  style_ina->body.border.width = 0;
  style_ina->text.color = LV_COLOR_WHITE;
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_INA, style_ina);
  lv_btnm_set_style(btnm, LV_BTNM_STYLE_BTN_REL, style_ina);

}
