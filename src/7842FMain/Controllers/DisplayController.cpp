#include "DisplayController.hpp"

DisplayController::DisplayController(lv_obj_t* parent) :
tabview(lv_tabview_create(parent, NULL))
{
  lv_color_t mainColor = LV_COLOR_HEX(0xFF7F00);

  lv_style_t* style_bg = new lv_style_t;
  lv_style_copy(style_bg, &lv_style_plain);
  style_bg->body.main_color = mainColor;
  style_bg->body.grad_color = mainColor;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BG, style_bg);

  lv_style_t* style_indic = new lv_style_t;
  lv_style_copy(style_indic, &lv_style_plain);
  style_indic->body.main_color = LV_COLOR_WHITE;
  style_indic->body.grad_color = LV_COLOR_WHITE;
  style_indic->body.padding.inner = 3;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_INDIC, style_indic);

  lv_style_t* style_btn_bg = new lv_style_t;
  lv_style_copy(style_btn_bg, &lv_style_transp);
  style_btn_bg->body.main_color = mainColor;
  style_btn_bg->body.grad_color = mainColor;
  style_btn_bg->body.padding.ver = 5;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_INDIC, style_btn_bg);

  // lv_style_t* style_rel = new lv_style_t;
  // lv_style_copy(style_rel, &lv_style_btn_tgl_rel);
  // style_rel->body.main_color = mainColor;
  // style_rel->body.grad_color = mainColor;
  // style_rel->body.border.color = LV_COLOR_WHITE;
  // style_rel->body.border.width = 3;
  // style_rel->body.border.opa = LV_OPA_100;
  // style_rel->body.radius = 15;
  // style_rel->text.color = LV_COLOR_WHITE;
  // lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_TGL_REL, style_rel);
  //
  // lv_style_t* style_pr = new lv_style_t;
  // lv_style_copy(style_pr, style_rel);
  // style_pr->body.main_color = LV_COLOR_WHITE;
  // style_pr->body.grad_color = LV_COLOR_WHITE;
  // style_pr->text.color = mainColor;
  // lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_TGL_PR, style_pr);
  // lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_PR, style_pr);
  //
  // lv_style_t* style_ina = new lv_style_t;
  // lv_style_copy(style_ina, &lv_style_btn_ina);
  // style_ina->body.main_color = mainColor;
  // style_ina->body.grad_color = mainColor;
  // style_ina->body.border.width = 0;
  // style_ina->text.color = LV_COLOR_WHITE;
  // lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_INA, style_ina);
  // lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_REL, style_ina);
}

DisplayController::~DisplayController() {
  lv_obj_del(tabview);
}


lv_obj_t* DisplayController::newTab(std::string name) {
  lv_obj_t* tab = lv_tabview_add_tab(tabview, name.c_str());
  lv_obj_align(tab, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
  return tab;
}
