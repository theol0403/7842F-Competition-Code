#pragma once
#include "objectTracking.hpp"

namespace lib7842
{

  class ObjectContainer
  {

  public:

    struct visionObj
    {
      int objSig = VISION_OBJECT_ERR_SIG;
      double objX = 0;
      double objY = 0;
      double objWidth = 0;
      double objHeight = 0;
      double objArea = 0;
      double objCenterX = 0;
      double objCenterY = 0;
    };

    enum class objAttr
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

    typedef typedef std::function<bool(const visionObj&, const visionObj&)> sortFunc_t;

    std::vector<visionObj> objects = {};

    ObjectContainer();

    void addObj(visionObj);
    void addObj(std::vector<visionObj>)

    visionObj getObjectByIndex(int);
    visionObj getObjectBySigIndex(int, int);

    double getObjAttrByIndex(objAttr, int);
    sortFunc_t createSort(objAttr, bool = true);
    void sort(sortFunc_t);

    void removeObjWith(objAttr, double, double);
    void removeObjWith(objAttr, double);
    void resize(int);
    void reset();

    void debugObjects(int);
    void debugErrorSig();

  };
}
