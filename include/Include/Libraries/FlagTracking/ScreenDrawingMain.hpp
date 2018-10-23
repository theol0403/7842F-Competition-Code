#include "main.h"
#pragma once



class screenDrawing
{
private:

  const flagSig_t m_flagSig;

  const int m_widthScale;
  const int m_heightScale;
  lv_obj_t* m_objectContainer = nullptr;


  //Simple Objects
  int m_simpleObjectCount = 0;

  lv_obj_t** m_simpleObjects = nullptr;
  lv_style_t m_simpleObjectStyle;
  lv_style_t m_blueSimpleObjectStyle;
  lv_style_t m_redSimpleObjectStyle;
  lv_style_t m_discardSimpleObjectStyle;




public:

  screenDrawing(flagSig_t, int, int);
  ~screenDrawing();

  void initContainer(int, int);

  void initSimpleObjects(int);
  void drawSimpleObjects(simpleObjects*);



};
