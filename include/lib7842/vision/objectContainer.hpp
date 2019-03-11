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

    ObjectContainer operator+(ObjectContainer);

    sortFunc_t makeSort(objAttr, bool = true);
    void sortBy(objAttr, bool = true);
    void sortBy(sortFunc_t);

    void print();
    void checkErrSig();

  };
}
