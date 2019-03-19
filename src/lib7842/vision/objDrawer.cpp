#include "objDrawer.hpp"

namespace lib7842
{

  lv_style_t ObjRenderer::defaultObjStyle = [] () {
    lv_style_copy(&defaultObjStyle, &lv_style_pretty_color);
    defaultObjStyle.body.main_color = LV_COLOR_BLACK;
    defaultObjStyle.body.grad_color = LV_COLOR_BLACK;
    defaultObjStyle.body.radius = 5;
    defaultObjStyle.body.border.color = LV_COLOR_WHITE;
    defaultObjStyle.body.border.width = 2;
    defaultObjStyle.body.border.opa = LV_OPA_100;
    return defaultObjStyle;
  } ();


  ObjRenderer::ObjRenderer(lv_obj_t* iparent, ObjContainer* icontainer) :
  parent(iparent), wScale(lv_obj_get_width(parent)/VISION_FOV_WIDTH), hScale(lv_obj_get_height(parent)/VISION_FOV_HEIGHT),
  container(icontainer), objStyle(defaultObjStyle)
  {
  }

  ObjRenderer::~ObjRenderer() {
    clear();
  }

  void ObjRenderer::expandTo(int size) {
    int diff = size - objects.size();
    for(int i = 0; i < diff; i++) {
      lv_obj_t* obj = lv_obj_create(parent, NULL);
      formatObj(obj);
      objects.push_back(obj);
    }
  }

  void ObjRenderer::formatObj(lv_obj_t* obj) {
    lv_obj_set_style(obj, &objStyle);
    lv_obj_set_hidden(obj, true);
  }

  void ObjRenderer::format() {
    for(lv_obj_t* obj : objects) formatObj(obj);
  }


  ObjRenderer &ObjRenderer::withStyle(lv_color_t main, lv_color_t border, lv_opa_t opa) {
    objStyle.body.main_color = main;
    objStyle.body.grad_color = main;
    objStyle.body.border.color = border;
    objStyle.body.opa = opa;
    format();
    return *this;
  }

  ObjRenderer &ObjRenderer::withStyle(int sig, lv_color_t main, lv_color_t border, lv_opa_t opa) {
    lv_style_t style;
    lv_style_copy(&style, &objStyle);
    style.body.main_color = main;
    style.body.grad_color = main;
    style.body.border.color = border;
    style.body.opa = opa;
    sigStyles.insert_or_assign(sig, style);
    return *this;
  }


  void ObjRenderer::draw() {
    expandTo(container.objects)
  }


  void ObjRenderer::clear() {
    for(lv_obj_t* obj : objects) {
      lv_obj_del(obj);
    }
    objects.clear();
  }




















}
