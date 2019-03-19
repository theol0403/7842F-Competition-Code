#pragma once
#include "objectTracking.hpp"
#include "objContainer.hpp"
#include <map>

namespace lib7842
{

  class ObjRenderer
  {
  private:

    static lv_style_t defaultObjStyle;

    lv_obj_t* parent = nullptr;
    const int wScale;
    const int hScale;
    ObjContainer* container = nullptr;
    lv_style_t objStyle;

    std::map<int, lv_style_t> sigStyles = {};
    std::vector<lv_obj_t*> dObjects = {};

  public:

    ObjRenderer(lv_obj_t*, ObjContainer*);
    ~ObjRenderer();

    void expandTo(int);
    void formatObj(lv_obj_t*);
    void format();

    ObjRenderer &withStyle(lv_color_t, lv_color_t = LV_COLOR_BLACK, lv_opa_t = LV_OPA_100);
    ObjRenderer &withStyle(int, lv_color_t, lv_color_t = LV_COLOR_BLACK, lv_opa_t = LV_OPA_100);

    void draw();
    void clear();

  };


  class ObjDrawer
  {
  private:

    lv_obj_t* dContainer = nullptr;
    lv_style_t style;

    std::vector<ObjRenderer> layers = {};

  public:

    ObjDrawer(lv_obj_t*);
    ~ObjDrawer();

    ObjDrawer &withStyle(lv_color_t, lv_color_t = LV_COLOR_BLACK, lv_opa_t = LV_OPA_100);
    ObjRenderer &withLayer(ObjContainer&);

    void draw();

  };

}
