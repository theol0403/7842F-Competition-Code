#include "DisplayController.hpp"

DisplayController::DisplayController(lv_obj_t* parent) :
tabview(lv_tabview_create(parent, NULL))
{
  lv_color_t mainColor = LV_COLOR_HEX(0xFF7F00);

  lv_style_t* style_bg = new lv_style_t;
  lv_style_copy(style_bg, &lv_style_plain);
  style_bg->body.main_color = mainColor;
  style_bg->body.grad_color = mainColor;
  style_bg->body.padding.ver = 0;
  style_bg->body.padding.hor = 0;
  style_bg->body.padding.inner = 0;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BG, style_bg);

  lv_style_t* style_indic = new lv_style_t;
  lv_style_copy(style_indic, &lv_style_plain_color);
  style_indic->body.main_color = LV_COLOR_WHITE;
  style_indic->body.grad_color = LV_COLOR_WHITE;
  style_indic->body.padding.inner = 3;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_INDIC, style_indic);

  lv_style_t* style_btn_bg = new lv_style_t;
  lv_style_copy(style_btn_bg, &lv_style_transp);
  style_btn_bg->body.main_color = mainColor;
  style_btn_bg->body.grad_color = mainColor;
  style_btn_bg->body.padding.ver = -10;
  style_btn_bg->body.padding.hor = 0;
  style_btn_bg->body.padding.inner = 1;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_BG, style_btn_bg);

  lv_style_t* style_rel = new lv_style_t;
  lv_style_copy(style_rel, &lv_style_btn_rel);
  style_rel->body.main_color = mainColor;
  style_rel->body.grad_color = mainColor;
  style_rel->body.border.width = 0;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_REL, style_rel);

  lv_style_t* style_tgl_rel = new lv_style_t;
  lv_style_copy(style_tgl_rel, &lv_style_btn_tgl_rel);
  style_tgl_rel->body.main_color = mainColor;
  style_tgl_rel->body.grad_color = mainColor;
  style_tgl_rel->body.border.color = LV_COLOR_WHITE;
  style_tgl_rel->body.border.width = 3;
  style_tgl_rel->body.radius = 0;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_TGL_REL, style_tgl_rel);

  lv_style_t* style_pr = new lv_style_t;
  lv_style_copy(style_pr, style_rel);
  style_pr->body.main_color = LV_COLOR_WHITE;
  style_pr->body.grad_color = LV_COLOR_WHITE;
  style_pr->text.color = mainColor;
  style_pr->body.border.width = 0;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_PR, style_pr);
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_TGL_PR, style_pr);

  lv_style_copy(&pageStyle, &lv_style_pretty_color);
  pageStyle.body.padding.hor = 0;
  pageStyle.body.padding.ver = -10;
  pageStyle.body.padding.inner = 0;

}

DisplayController::~DisplayController() {
  lv_obj_del(tabview);
}


lv_obj_t* DisplayController::newTab(std::string name) {
  lv_obj_t* page = lv_tabview_add_tab(tabview, name.c_str());
  lv_page_set_sb_mode(page, LV_SB_MODE_OFF);
  lv_page_set_style(page, LV_PAGE_STYLE_BG, &pageStyle);
  //lv_obj_align(page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
  return page;
}
