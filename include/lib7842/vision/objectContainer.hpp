#pragma once
#include "objectTracking.hpp"
#include "visionObj.hpp"

namespace lib7842
{

  class ObjectContainer
  {

  public:

    typedef std::function<bool(const visionObj&, const visionObj&)> sortFunc_t;
    using ObjList = std::vector<std::vector<visionObj>::iterator>;

    std::vector<visionObj> objects = {};

    void resize(int);
    void shrinkTo(int);
    void reset();

    void addObj(visionObj);
    void addObj(std::vector<visionObj>);

    visionObj getTotalObj();
    visionObj getAvgObj();

    double getAttrIndex(objAttr, int);
    double getTotalAttr(objAttr);
    double getAvgAttr(objAttr);

    ObjList findIndex(int);
    ObjList findIndexRange(int, int);
    ObjList findSigIndex(int, int);
    ObjList findSigIndexRange(int, int, int);
    ObjList findWith(objAttr, double);
    ObjList findWithout(objAttr, double);
    ObjList findWithRange(objAttr, double, double);
    ObjList findWithoutRange(objAttr, double, double);

    visionObj getIndex(int);
    visionObj getSigIndex(int, int);
    visionObj getWith(objAttr, double);
    visionObj getWithout(objAttr, double);
    visionObj getWithRange(objAttr, double, double);
    visionObj getWithoutRange(objAttr, double, double);

    void removeIndex(int);
    void removeIndexRange(int, int);
    void removeSigIndex(int, int);
    void removeSigIndexRange(int, int, int);
    void removeWith(objAttr, double);
    void removeWithout(objAttr, double);
    void removeWithRange(objAttr, double, double);
    void removeWithoutRange(objAttr, double, double);

    ObjectContainer extractIndex(int);
    ObjectContainer extractIndexRange(int, int);
    ObjectContainer extractSigIndex(int, int);
    ObjectContainer extractSigIndexRange(int, int, int);
    ObjectContainer extractWith(objAttr, double);
    ObjectContainer extractWithout(objAttr, double);
    ObjectContainer extractWithRange(objAttr, double, double);
    ObjectContainer extractWithoutRange(objAttr, double, double);

    sortFunc_t makeSort(objAttr, bool = true);
    void sortBy(objAttr, bool = true);
    void sortBy(sortFunc_t);

    void print();
    void checkErrSig();

    ObjectContainer operator+(ObjectContainer);

  };
}
