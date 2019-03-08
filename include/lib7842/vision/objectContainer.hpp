#pragma once
#include "objectTracking.hpp"
#include "visionObj.hpp"

namespace lib7842
{

  class ObjectContainer
  {

  public:

    typedef std::function<bool(const visionObj&, const visionObj&)> sortFunc_t;

    std::vector<visionObj> objects = {};

    void resize(int);
    void shrinkTo(int);
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
