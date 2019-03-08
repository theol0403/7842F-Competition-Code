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
    ObjList findWithAttr(objAttr, double);
    ObjList findWithoutAttr(objAttr, double);
    ObjList findWithAttrRange(objAttr, double, double);
    ObjList findWithoutAttrRange(objAttr, double, double);

    visionObj getIndex(int);
    visionObj getSigIndex(int, int);
    visionObj getWithAttr(objAttr, double);
    visionObj getWithoutAttr(objAttr, double);
    visionObj getWithAttrRange(objAttr, double, double);
    visionObj getWithoutAttrRange(objAttr, double, double);

    ObjList removeIndex(int);
    ObjList removeIndexRange(int, int);
    ObjList removeSigIndex(int, int);
    ObjList removeSigIndexRange(int, int, int);
    ObjList removeWithAttr(objAttr, double);
    ObjList removeWithoutAttr(objAttr, double);
    ObjList removeWithAttrRange(objAttr, double, double);
    ObjList removeWithoutAttrRange(objAttr, double, double);

    ObjectContainer extractIndex(int);
    ObjectContainer extractIndexRange(int, int);
    ObjectContainer extractSigIndex(int, int);
    ObjectContainer extractSigIndexRange(int, int, int);
    ObjectContainer extractWithAttr(objAttr, double);
    ObjectContainer extractWithoutAttr(objAttr, double);
    ObjectContainer extractWithAttrRange(objAttr, double, double);
    ObjectContainer extractWithoutAttrRange(objAttr, double, double);

    sortFunc_t makeSort(objAttr, bool = true);
    void sortBy(objAttr, bool = true);
    void sortBy(sortFunc_t);

    void print();
    void checkErrSig();

    ObjectContainer operator+(ObjectContainer);

  };
}
