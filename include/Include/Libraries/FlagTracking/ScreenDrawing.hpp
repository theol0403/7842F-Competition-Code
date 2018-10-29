#include "main.h"
#pragma once



class ScreenDrawing
{
private:

  const flagSig_t m_flagSig;

  const int m_widthScale;
  const int m_heightScale;
  lv_obj_t* m_objectContainer = nullptr;


  //Simple Objects
  int m_screenObjectLength = 0;

  lv_obj_t** m_screenObjects = nullptr;
  lv_style_t m_defaultObjectStyle;
  lv_style_t m_blueObjectStyle;
  lv_style_t m_redObjectStyle;
  lv_style_t m_discardObjectStyle;




public:

  ScreenDrawing(flagSig_t, int, int);
  ~ScreenDrawing();

  void initContainer(int, int);

  void initSimpleObjects(int);
  void drawSimpleObjects(simpleObjects_t*, int currentCount = -1);




};
