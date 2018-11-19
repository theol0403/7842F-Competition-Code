#pragma once
#include "main.h"
#include "FlagTrackingMain.hpp"

class ScreenDrawing
{
private:

  const flagSig_t m_flagSig;

  const int m_widthScale;
  const int m_heightScale;
  lv_obj_t* m_objectContainer = nullptr;
  lv_style_t m_objectContainerStyle;

  lv_style_t m_defaultObjectStyle;
  lv_style_t m_discardObjectStyle;


public:

  ScreenDrawing(flagSig_t, int, int);
  ~ScreenDrawing();


  screenObjects_t createSimpleObjects(int);
  lv_style_t createStyle(lv_color_t);

  void drawSimpleObjects(screenObjects_t, simpleObjects_t*, int = -1);


};
