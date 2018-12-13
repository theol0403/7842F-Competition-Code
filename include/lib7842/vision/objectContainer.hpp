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
    double objArea = 0; // Avg of width and height
    double objCenterX = 0;
    double objCenterY = 0;
    bool discardObject = false;
  };

  enum class sortModes_t
  {
    objSig,
    objX,
    objY,
    objWidth,
    objHeight,
    objArea,
    objCenterX,
    objCenterY
  };


  class ObjectContainer
  {
  private:

    static const simpleObjects_t emptyObject;

  public:

    const int arrayLength;
    std::vector<simpleObjects_t> objectArray;

    std::vector<lv_obj_t*> screenArray;
    std::array<lv_style_t, NUM_SIGNATURES+1> styleArray;

    int currentCount = 0;


    ObjectContainer(int);
    ObjectContainer(int, lib7842::ObjectDrawing&);

    ~ObjectContainer();

    void setContainerStyle(lv_color_t, lv_color_t = LV_COLOR_YELLOW, lv_opa_t = LV_OPA_100);
    void setSigStyle(int, lv_color_t, lv_color_t = LV_COLOR_YELLOW, lv_opa_t = LV_OPA_100);

    simpleObjects_t getObject(int);
    simpleObjects_t getObject(int, int);

    void filterAreaRange(double, double, bool = true); //Example 25
    void filterAvgArea(double, bool = true); //Example 0.5
    void filterProp(double, double, bool = true); //Width:Height = 1:x Example 0.3, 0.8
    void discardObjects();

    void debugObjects(int);
    void debugErrorSig();

    void sortBy(sortModes_t, bool = true);
    void shrinkTo(int);

    static void copyObjects(ObjectContainer&, ObjectContainer&, int = -1);

  };
}
