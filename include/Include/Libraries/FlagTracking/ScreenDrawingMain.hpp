#include "main.h"
#pragma once



class screenDrawing
{
private:

  const int m_widthScale;
  const int m_heightScale;

  lv_obj_t * m_objectContainer;




  lv_obj_t ** m_visionObjects;
  lv_style_t m_objectStyle;
  lv_style_t m_blueObjectStyle;
  lv_style_t m_redObjectStyle;
  lv_style_t m_discardObjectStyle;




public:

  screenDrawing(int, int, int);
  void initContainer(int, int);
  void initVisionObjects(int);



};
