#pragma once
#include "objectTracking.hpp"
#include "visionObj.hpp"

namespace lib7842
{

  class ObjectContainer
  {

  public:

    typedef std::function<bool(const visionObj&, const visionObj&)> sortFunc_t;
    using ObjIt = std::vector<std::vector<visionObj>::iterator>;

    std::vector<visionObj> objects = {};

    void resize(int);
    void shrinkTo(int);
    void reset();

    void addObj(visionObj);
    void addObj(std::vector<visionObj>);

    ObjIt findWithIndex(int);
    ObjIt findWithIndex(int, int);
    ObjIt findWithSigIndex(int, int);
    ObjIt findWithAttr(objAttr, double);
    ObjIt findWithoutAttr(objAttr, double);
    ObjIt findWithAttr(objAttr, double, double);
    ObjIt findWithoutAttr(objAttr, double, double);

    visionObj getWithIndex(int);
    visionObj getWithSigIndex(int, int);
    visionObj getTotalObj();
    visionObj getAvgObj();
    
    double getObjAttr(objAttr, int);
    double getTotalAttr(objAttr);
    double getAvgAttr(objAttr);

    void removeObjIndex(int);
    void removeObjIndex(int, int);
    void removeObjWith(objAttr, double);
    void removeObjWithout(objAttr, double);
    void removeObjWith(objAttr, double, double);
    void removeObjWithout(objAttr, double, double);

    sortFunc_t makeSort(objAttr, bool = true);
    void sortBy(objAttr, bool = true);
    void sortBy(sortFunc_t);

    void print();
    void checkErrSig();

    ObjectContainer operator+(ObjectContainer);
    ObjectContainer extractIndex(int);
    ObjectContainer extractIndex(int, int);
    ObjectContainer extractWith(objAttr, double);
    ObjectContainer extractWithout(objAttr, double);
    void removeObjWith(objAttr, double, double);
    void removeObjWithout(objAttr, double, double);

  };
}
