#pragma once
#include "objectTracking.hpp"

namespace lib7842
{

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

      visionObj operator+(visionObj);
      visionObj operator-(visionObj);
      visionObj operator*(visionObj);
      visionObj operator/(visionObj);

      visionObj operator+(double);
      visionObj operator-(double);
      visionObj operator*(double);
      visionObj operator/(double);

      double getAttr(objAttr) const;
    };

    typedef std::function<bool(const visionObj&, const visionObj&)> sortFunc_t;

    std::vector<visionObj> objects = {};

    void resize(int);
    void reset();

    void addObj(visionObj);
    void addObj(std::vector<visionObj>);

    visionObj getObjByIndex(int);
    visionObj getObjBySigIndex(int, int);
    visionObj getTotalObj();
    visionObj getAvgObj();

    double getObjAttr(objAttr, int);
    double getTotalAttr(objAttr);
    double getAvgAttr(objAttr);

    void removeObjIndex(int);
    void removeObjWith(objAttr, double);
    void removeObjWithout(objAttr, double);
    void removeObjWith(objAttr, double, double);
    void removeObjWithout(objAttr, double, double);

    sortFunc_t makeSort(objAttr, bool = true);
    void sortBy(objAttr, bool = true);
    void sortBy(sortFunc_t);

    void debugObjects(int);
    void debugErrorSig();

  };
}
