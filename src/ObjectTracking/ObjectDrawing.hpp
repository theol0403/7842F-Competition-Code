#pragma once
#include "ObjectTrackingMain.hpp"

class ObjectDrawing
{
private:

  const objectSig_t m_flagSig;

  const int m_widthScale;
  const int m_heightScale;
  lv_obj_t* m_objectContainer = nullptr;
  lv_style_t m_objectContainerStyle;

  lv_style_t m_defaultObjectStyle;
  lv_style_t m_discardObjectStyle;


public:

  ObjectDrawing(objectSig_t, int, int);
  ~ObjectDrawing();

  objectContainer_t* createObjectContainer(int, bool = false);

  lv_obj_t** createSimpleObjects(int);

  lv_style_t* createStyle(lv_color_t, lv_color_t = LV_COLOR_YELLOW, lv_opa_t = LV_OPA_100);

  void drawSimpleObjects(objectContainer_t*);


};
