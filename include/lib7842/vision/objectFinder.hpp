#pragma once
#include "objectTracking.hpp"
#include "ObjectContainer.hpp"

namespace lib7842
{

  class ObjectFinder
  {

  public:

    struct objList {
      ObjectContainer* container = nullptr;
      std::vector<int> indexes = {};
    };

    std::vector<objList> lists = {};

    ObjectFinder(ObjectContainer&);

    ObjectFinder &find(ObjectContainer&);

    ObjectFinder &withAttr(objAttr, double);
    ObjectFinder &withoutAttr(objAttr, double);
    ObjectFinder &withAttrRange(objAttr, double, double);
    ObjectFinder &withoutAttrRange(objAttr, double, double);

    visionObj get(int);
    visionObj getTotalObj();
    visionObj getAvgObj();

    ObjectContainer extract();
    ObjectContainer extract(int);
    ObjectContainer extract(int, int);

    void remove();
    void remove(int);
    void remove(int, int);

    void sort(objAttr, bool = true);

  };
}
