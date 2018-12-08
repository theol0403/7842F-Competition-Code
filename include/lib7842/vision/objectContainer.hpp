#pragma once
#include "objectTracking.hpp"

namespace lib7842
{

  struct simpleObjects_t
  {
    int objSig = VISION_OBJECT_ERR_SIG;
    double objX = 0;
    double objY = 0;
    double objWidth = 0;
    double objHeight = 0;
    double objSize = 0; // Avg of width and height
    double objCenterX = 0;
    double objCenterY = 0;
    bool discardObject = false;
  };


  class ObjectContainer
  {
  private:


  public:


    ObjectContainer(int);
    ObjectContainer(int, lib7842::ObjectDrawing&);

    ~ObjectContainer();

    void setContainerStyle(lv_color_t, lv_color_t = LV_COLOR_YELLOW, lv_opa_t = LV_OPA_100);
    void setSigStyle(int, lv_color_t, lv_color_t = LV_COLOR_YELLOW, lv_opa_t = LV_OPA_100);

    const int arrayLength;
    std::vector<simpleObjects_t> objectArray;

    std::vector<lv_obj_t*> screenArray;
    std::array<lv_style_t, NUM_SIGNATURES+1> styleArray;

    int currentCount = 0;

    int getArrayLength();


  };

}
