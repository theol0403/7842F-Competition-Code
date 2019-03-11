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

    void addObj(visionObj);
    void addObj(std::vector<visionObj>);

    visionObj get(int);
    visionObj getTotal();
    visionObj getAvg();

    ObjectContainer copy();

    ObjectContainer &resize(int);
    ObjectContainer &trim(int);

    ObjectContainer &remove(int);
    ObjectContainer &remove(int, int);
    ObjectContainer &removeWith(objAttr, double);
    ObjectContainer &removeWithout(objAttr, double);
    ObjectContainer &removeWith(objAttr, double, double);
    ObjectContainer &removeWithout(objAttr, double, double);

    ObjectContainer &sortBy(objAttr, bool = true);

    sortFunc_t makeSort(objAttr, bool = true);
    void sortBy(sortFunc_t);

    void reset();
    void print();
    void checkErrSig();

    ObjectContainer operator+(ObjectContainer);

  };
}
