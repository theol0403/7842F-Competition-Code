#pragma once
#include "objContainer.hpp"
#include <map>

namespace lib7842
{

  class ObjRenderer
  {
  private:

    static lv_style_t defaultObjStyle;

    lv_obj_t* parent = nullptr;
    const double wScale;
    const double hScale;
    ObjContainer* oContainer = nullptr;
    lv_style_t objStyle;

    std::map<int, lv_style_t> sigStyles = {};
    std::vector<lv_obj_t*> dObjects = {};

  public:

    ObjRenderer(lv_obj_t*, ObjContainer*);
    ~ObjRenderer();

    void expandTo(int);
    void formatObj(lv_obj_t*);
    void format();

    ObjRenderer &withStyle(lv_color_t, lv_color_t = LV_COLOR_WHITE, lv_opa_t = LV_OPA_100);
    ObjRenderer &withStyle(int, lv_color_t, lv_color_t = LV_COLOR_WHITE, lv_opa_t = LV_OPA_100);

    void draw();
    void clear();

    friend class ObjDrawer;

  };


  class ObjDrawer
  {
  private:

    lv_obj_t* dContainer = nullptr;
    lv_style_t cStyle;
    lv_obj_t* infoLabel;

    std::vector<ObjRenderer> layers = {};

  public:

    ObjDrawer(lv_obj_t*);
    ~ObjDrawer();

    ObjDrawer &withContainerStyle(lv_color_t, lv_color_t = LV_COLOR_WHITE, lv_opa_t = LV_OPA_100);
    ObjRenderer &withLayer(ObjContainer&);

    void draw(ObjContainer&);

  };

}
