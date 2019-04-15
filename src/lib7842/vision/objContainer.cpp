#include "objContainer.hpp"
#include <cassert>
#include <algorithm>
#include <numeric>

namespace lib7842
{

  void ObjContainer::addObj(visionObj obj) {
    objects.push_back(obj);
  }

  void ObjContainer::addObj(std::vector<visionObj> objs) {
    for(auto&& obj : objs) addObj(obj);
  }

  visionObj ObjContainer::get(int index) {
    if(index < objects.size()) {
      return objects.at(index);
    } else {
      std::cerr << "get: Invalid Index" << std::endl;
      return {};
    }
  }

  visionObj ObjContainer::getTotal() {
    return std::accumulate(objects.begin(), objects.end(), visionObj{});
  }

  visionObj ObjContainer::getAvg() {
    return getTotal() / objects.size();
  }


  ObjContainer::removeFunc_t ObjContainer::makeRemove(objAttr attr, std::function<bool(double, double)> compare, double val) {
    return [=](const visionObj& obj) {
      return compare(obj.getAttr(attr), val);
    };
  }

  ObjContainer::removeFunc_t ObjContainer::makeRemove(objAttr attr, bool inRange, double min, double max) {
    return [=](const visionObj& obj) {
      return (min < obj.getAttr(attr) && obj.getAttr(attr) < max) == inRange;
    };
  }

  void ObjContainer::removeBy(removeFunc_t removeFunc) {
    objects.erase(std::remove_if(objects.begin(), objects.end(), removeFunc), objects.end());
  }


  ObjContainer::sortFunc_t ObjContainer::makeSort(objAttr attr, bool decending) {
    return [=](const visionObj& first, const visionObj& second) {
      if(decending) {
        return first.getAttr(attr) > second.getAttr(attr);
      } else {
        return first.getAttr(attr) < second.getAttr(attr);
      }
    };
  }

  void ObjContainer::sortBy(sortFunc_t sortFunc) {
    std::stable_sort(objects.begin(), objects.end(), sortFunc);
  }


  ObjContainer ObjContainer::copy() {
    return *this;
  }


  ObjContainer &ObjContainer::resize(int size) {
    objects.resize(size);
    return *this;
  }

  ObjContainer &ObjContainer::trim(int size) {
    if(size < objects.size()) resize(size);
    return *this;
  }

  ObjContainer &ObjContainer::remove(int index) {
    if(index < objects.size()) {
      objects.erase(objects.begin() + index);
    }
    return *this;
  }

  ObjContainer &ObjContainer::remove(int start, int end) {
    end = end > objects.size() ? objects.size() : end;
    if(start < objects.size()) {
      objects.erase(objects.begin() + start, objects.begin() + end);
    }
    return *this;
  }


  ObjContainer &ObjContainer::removeBy(objAttr attr, std::function<bool(double, double)> compare, double val) {
    removeBy(makeRemove(attr, compare, val));
    return *this;
  }

  ObjContainer &ObjContainer::removeWith(objAttr attr, double val) {
    removeBy(makeRemove(attr, std::equal_to<>(), val));
    return *this;
  }

  ObjContainer &ObjContainer::removeWithout(objAttr attr, double val) {
    removeBy(makeRemove(attr, std::not_equal_to<>(), val));
    return *this;
  }

  ObjContainer &ObjContainer::removeWith(objAttr attr, double min, double max) {
    removeBy(makeRemove(attr, true, min, max));
    return *this;
  }

  ObjContainer &ObjContainer::removeWithout(objAttr attr, double min, double max) {
    removeBy(makeRemove(attr, false, min, max));
    return *this;
  }


  ObjContainer &ObjContainer::sortBy(objAttr attr, bool decending) {
    sortBy(makeSort(attr, decending));
    return *this;
  }


  void ObjContainer::reset() {
    resize(0);
  }


  void ObjContainer::print() {
    std::cout << "Count:" << objects.size() << std::endl;
    for(auto&& obj : objects) obj.print();
  }


  void ObjContainer::checkErrSig() {
    for(auto&& obj : objects) { assert(obj.sig != VISION_OBJECT_ERR_SIG); }
  }

  int ObjContainer::getCount() {
    return objects.size();
  }

  ObjContainer ObjContainer::operator+(ObjContainer rhs) {
    addObj(rhs.objects);
    return *this;
  }

}
