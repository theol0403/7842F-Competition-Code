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

    const int wScale;
    const int hScale;

    lv_obj_t* parent = nullptr;
    ObjContainer* container = nullptr;

    lv_style_t objStyle;
    std::map<int, lv_style_t> objects = {};

    std::vector<lv_obj_t*> objects = {};

  public:

    ObjRenderer(lv_obj_t*, ObjContainer*);
    ~ObjRenderer();

    ObjRenderer &withStyle(lv_color_t, lv_color_t = LV_COLOR_BLACK, lv_opa_t = LV_OPA_100);
    ObjRenderer &withStyle(int, lv_color_t, lv_color_t = LV_COLOR_BLACK, lv_opa_t = LV_OPA_100);

    void draw();
    void clear();

  };


  class ObjDrawer
  {
  private:

    lv_obj_t* container = nullptr;
    lv_style_t style;

    std::vector<ObjRenderer> layers = {};

  public:

    ObjDrawer(lv_obj_t*);
    ~ObjDrawer();

    ObjDrawer &withStyle(lv_color_t, lv_color_t = LV_COLOR_BLACK, lv_opa_t = LV_OPA_100);

    ObjRenderer &withLayer(ObjContainer&);

  };

}
