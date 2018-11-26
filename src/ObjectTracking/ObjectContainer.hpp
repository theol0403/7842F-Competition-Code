#pragma once
#include "ObjectTrackingMain.hpp"


class ObjectContainer
{
private:


public:


  ObjectContainer(int);
  ObjectContainer(int, ObjectDrawing&);

  ~ObjectContainer();

  void setContainerStyle(lv_color_t, lv_color_t = LV_COLOR_YELLOW, lv_opa_t = LV_OPA_100);
  void setSigStyle(int, lv_color_t, lv_color_t = LV_COLOR_YELLOW, lv_opa_t = LV_OPA_100);

  const int m_arrayLength;
  std::vector<simpleObjects_t> objectArray;

  std::vector<lv_obj_t*> screenArray;
  std::array<lv_style_t, NUM_SIGNATURES+1> styleArray;

  int currentCount = 0;

  int getArrayLength();


};
