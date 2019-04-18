#pragma once
#include "main.h"
#include "visionObj.hpp"
#include <functional>

namespace lib7842
{

  class ObjContainer
  {

  public:

    typedef std::function<bool(const visionObj&)> removeFunc_t;
    typedef std::function<bool(const visionObj&, const visionObj&)> sortFunc_t;

    std::vector<visionObj> objects = {};

    void addObj(visionObj);
    void addObj(std::vector<visionObj>);

    visionObj get(int);
    visionObj getTotal();
    visionObj getAvg();

    removeFunc_t makeRemove(objAttr, std::function<bool(double, double)>, double);
    removeFunc_t makeRemove(objAttr, bool, double, double);
    void removeBy(removeFunc_t);

    sortFunc_t makeSort(objAttr, bool = true);
    void sortBy(sortFunc_t);

    ObjContainer copy();

    ObjContainer &resize(int);
    ObjContainer &shrinkTo(int);
    ObjContainer &remove(int);
    ObjContainer &remove(int, int);

    ObjContainer &removeBy(objAttr, std::function<bool(double, double)>, double);
    ObjContainer &removeWith(objAttr, double);
    ObjContainer &removeWithout(objAttr, double);
    ObjContainer &removeWith(objAttr, double, double);
    ObjContainer &removeWithout(objAttr, double, double);

    ObjContainer &sortBy(objAttr, bool = true);

    ObjContainer &snapTo(objAttr, double);
    ObjContainer &snapToGrid(objAttr, int, double);
    ObjContainer &snapToGridX(int);
    ObjContainer &snapToGridY(int);

    void reset();
    void print();
    int getCount();
    void checkErrSig();

    ObjContainer operator+(ObjContainer);

  };
}
