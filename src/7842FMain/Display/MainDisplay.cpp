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
  style_rel->body.border.opa = LV_OPA_100;
  style_rel->body.border.color = LV_COLOR_WHITE;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_REL, style_rel);

  lv_style_t* style_tgl_rel = new lv_style_t;
  lv_style_copy(style_tgl_rel, &lv_style_btn_tgl_rel);
  style_tgl_rel->body.main_color = mainColor;
  style_tgl_rel->body.grad_color = mainColor;
  style_tgl_rel->text.color = LV_COLOR_WHITE;
  style_tgl_rel->body.radius = 0;
  style_tgl_rel->body.border.width = 3;
  style_tgl_rel->body.border.opa = LV_OPA_100;
  style_tgl_rel->body.border.color = LV_COLOR_WHITE;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_TGL_REL, style_tgl_rel);

  lv_style_t* style_pr = new lv_style_t;
  lv_style_copy(style_pr, style_rel);
  style_pr->body.main_color = LV_COLOR_WHITE;
  style_pr->body.grad_color = LV_COLOR_WHITE;
  style_pr->text.color = mainColor;
  style_pr->body.border.width = 0;
  style_pr->text.color = mainColor;
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_PR, style_pr);
  lv_tabview_set_style(tabview, LV_TABVIEW_STYLE_BTN_TGL_PR, style_pr);

  lv_style_copy(&pageStyle, &lv_style_pretty_color);
  pageStyle.body.padding.ver = -10;
  pageStyle.body.padding.hor = 0;
  pageStyle.body.padding.inner = 0;
  pageStyle.body.main_color = mainColor;
  pageStyle.body.grad_color = mainColor;
  pageStyle.body.border.color = LV_COLOR_WHITE;
  pageStyle.body.border.width = 3;
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

  //the task needs to know information
  using Passer = std::tuple<MainDisplay*, const lv_img_t*, int>;

  void (*func)(void*) = [](void* input){
    auto [display, imgPtr, time] = *static_cast<Passer*>(input);

    lv_obj_t* parent = lv_layer_top();

    lv_obj_t* overlay = lv_obj_create(parent, NULL);
    lv_obj_set_size(overlay, lv_obj_get_width(parent), lv_obj_get_height(parent));

    lv_style_t style;
    lv_style_copy(&style, &lv_style_pretty_color);
    style.body.main_color = display->mainColor;
    style.body.grad_color = display->mainColor;
    style.body.border.width = 3;
    style.body.border.color = LV_COLOR_WHITE;
    style.body.border.opa = LV_OPA_100;
    style.body.radius = 0;
    lv_obj_set_style(overlay, &style);

    lv_obj_t* img = lv_img_create(overlay, NULL);
    lv_img_set_src(img, imgPtr);
    lv_obj_align(img, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_style_t imgStyle;
    lv_style_copy(&imgStyle, &lv_style_plain);
    lv_img_set_style(img, &imgStyle);

    lv_anim_t bAnim;
    bAnim.var = overlay; bAnim.start = 0; bAnim.end = 255;
    bAnim.fp = [](void* ioverlay, int32_t val)
    {
      lv_obj_t* overlay = static_cast<lv_obj_t*>(ioverlay);
      lv_style_t* parentStyle = lv_obj_get_style(overlay);
      parentStyle->body.border.opa = val;
      lv_obj_set_style(overlay, parentStyle);
    };
    bAnim.path = lv_anim_path_linear;
    bAnim.time = time/2;
    bAnim.end_cb = NULL; bAnim.act_time = 0; bAnim.playback = 1; bAnim.playback_pause = 0; bAnim.repeat = 0; bAnim.repeat_pause = 0;
    lv_anim_create(&bAnim);

    lv_anim_t iAnim;
    iAnim.var = img; iAnim.start = 0; iAnim.end = 255;
    iAnim.fp = [](void* iimg, int32_t val)
    {
      lv_obj_t* img = static_cast<lv_obj_t*>(iimg);
      lv_style_t* imgStyle = lv_img_get_style(img);
      imgStyle->image.opa = val;
      lv_img_set_style(img, imgStyle);
    };
    iAnim.path = lv_anim_path_linear;
    iAnim.time = time/3.0;
    iAnim.end_cb = NULL; iAnim.act_time = 0; iAnim.playback = 1; iAnim.playback_pause = time/3.0; iAnim.repeat = 0; iAnim.repeat_pause = 0;
    lv_anim_create(&iAnim);

    pros::delay(time);

    lv_obj_del(overlay);
    delete static_cast<Passer*>(input);

    pros::c::task_delete(CURRENT_TASK);
    std::cerr << "Splashscreen Not Deleted" << std::endl;
  };

  Passer* passer = new Passer(this, imgPtr, time);
  pros::c::task_create(func, passer, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");

}
