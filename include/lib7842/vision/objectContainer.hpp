#pragma once
#include "objectTracking.hpp"
#include "visionObj.hpp"
#include <functional>

namespace lib7842
{

  class ObjectContainer
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
    //visionObj getLargest();

    removeFunc_t makeRemove(objAttr, std::function<bool(double, double)>, double);
    removeFunc_t makeRemove(objAttr, bool, double, double);
    void removeBy(removeFunc_t);

    sortFunc_t makeSort(objAttr, bool = true);
    void sortBy(sortFunc_t);

    ObjectContainer copy();

    ObjectContainer &resize(int);
    ObjectContainer &trim(int);
    ObjectContainer &remove(int);
    ObjectContainer &remove(int, int);

    ObjectContainer &removeBy(objAttr, std::function<bool(double, double)>, double);
    ObjectContainer &removeWith(objAttr, double);
    ObjectContainer &removeWithout(objAttr, double);
    ObjectContainer &removeWith(objAttr, double, double);
    ObjectContainer &removeWithout(objAttr, double, double);

    ObjectContainer &sortBy(objAttr, bool = true);

    void reset();
    void print();
    void checkErrSig();

    ObjectContainer operator+(ObjectContainer);

  };
}
