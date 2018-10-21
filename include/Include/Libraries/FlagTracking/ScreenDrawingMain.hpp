#include "main.h"
#pragma once



class screenDrawing
{
private:

  const int m_containerWidth;
  const int m_containerHeight;
  const int m_widthScale;
  const int m_heightScale;

  lv_obj_t * m_objectContainer;

  lv_style_t blueObjectStyle;
  lv_style_t redObjectStyle;
  lv_style_t objectStyle;


  lv_obj_t * m_visionObjects;



public:

  screenDrawing(int, int, int);
  void initVisionObjects(int);



};
