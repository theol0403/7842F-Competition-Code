#include "main.h"
#pragma once



class screenDrawing
{
private:

  const int m_widthScale;
  const int m_heightScale;

  lv_obj_t * m_objectContainer;


  const flagSig_t m_flagSig;

  lv_obj_t ** m_screenObjects;
  lv_style_t m_objectStyle;
  lv_style_t m_blueObjectStyle;
  lv_style_t m_redObjectStyle;
  lv_style_t m_discardObjectStyle;




public:

  screenDrawing(flagSig_t, int, int, int);
  void initContainer(int, int);
  void initVisionObjects(int);
  void drawVisionObjects(visionObjects*, int);



};
