#include "MainDisplay.hpp"

MainDisplay::MainDisplay(lv_obj_t* parent, lv_color_t imainColor) :
tabview(lv_tabview_create(parent, NULL)), mainColor(imainColor)
{
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
  style_btn_bg->text.color = LV_COLOR_WHITE;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_BG, style_btn_bg);

  lv_style_t* style_rel = new lv_style_t;
  lv_style_copy(style_rel, &lv_style_btn_rel);
  style_rel->body.main_color = mainColor;
  style_rel->body.grad_color = mainColor;
  style_rel->body.border.width = 0;
  style_rel->text.color = LV_COLOR_WHITE;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_REL, style_rel);

  lv_style_t* style_tgl_rel = new lv_style_t;
  lv_style_copy(style_tgl_rel, &lv_style_btn_tgl_rel);
  style_tgl_rel->body.main_color = mainColor;
  style_tgl_rel->body.grad_color = mainColor;
  style_tgl_rel->body.border.color = LV_COLOR_WHITE;
  style_tgl_rel->body.border.width = 3;
  style_tgl_rel->body.radius = 0;
  style_tgl_rel->text.color = LV_COLOR_WHITE;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_TGL_REL, style_tgl_rel);

  lv_style_t* style_pr = new lv_style_t;
  lv_style_copy(style_pr, style_rel);
  style_pr->body.main_color = LV_COLOR_WHITE;
  style_pr->body.grad_color = LV_COLOR_WHITE;
  style_pr->text.color = mainColor;
  style_pr->body.border.width = 0;
  style_pr->text.color = LV_COLOR_WHITE;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_PR, style_pr);
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_TGL_PR, style_pr);

  lv_style_copy(&pageStyle, &lv_style_pretty_color);
  pageStyle.body.padding.ver = -10;
  pageStyle.body.padding.hor = 0;
  pageStyle.body.padding.inner = 0;
  pageStyle.body.main_color = mainColor;
  pageStyle.body.grad_color = mainColor;
  pageStyle.body.border.color = LV_COLOR_WHITE;
  pageStyle.body.border.width = 2;
  pageStyle.body.border.opa = LV_OPA_100;
  pageStyle.body.radius = 0;

}

MainDisplay::~MainDisplay() {
  lv_obj_del(tabview);
}


lv_obj_t* MainDisplay::newTab(std::string name) {
  lv_obj_t* page = lv_tabview_add_tab(tabview, name.c_str());
  lv_page_set_sb_mode(page, LV_SB_MODE_OFF);
  lv_page_set_style(page, LV_PAGE_STYLE_BG, &pageStyle);
  lv_obj_align(page, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
  return page;
}

lv_obj_t* MainDisplay::getParent() {
  return lv_obj_get_parent(tabview);
}


void MainDisplay::splashScreen(const lv_img_t* imgPtr, int time) {

  std::function<void(void*)> func = [=](void*){
    lv_obj_t* parent = getParent();

    lv_obj_t* overlay = lv_obj_create(parent, NULL);
    lv_obj_set_size(overlay, lv_obj_get_width(parent), lv_obj_get_height(parent));

    lv_style_t style;
    lv_style_copy(&style, &lv_style_pretty_color);
    style.body.main_color = mainColor;
    style.body.grad_color = mainColor;
    lv_obj_set_style(overlay, &style);

    lv_obj_t* img = lv_img_create(overlay, NULL);
    lv_img_set_src(img, imgPtr);
    lv_obj_align(img, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    pros::delay(time);

    lv_obj_del(overlay);

    pros::c::task_delete(CURRENT_TASK);
  };

  typedef void function_t(void*);
  function_t* funcPtr = func.target<function_t>();
  pros::c::task_create(funcPtr, nullptr, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");

}
