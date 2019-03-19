#include "objContainer.hpp"

namespace lib7842
{

  void ObjContainer::addObj(visionObj obj) {
    objects.push_back(obj);
  }

  void ObjContainer::addObj(std::vector<visionObj> objs) {
    for(visionObj &obj : objs) addObj(obj);
  }

  visionObj ObjContainer::get(int index) {
    try {
      return objects.at(index);
    } catch(std::out_of_range) {
      std::cerr << "get: Invalid Index\n";
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
      return (min < obj.getAttr(attr) || obj.getAttr(attr) > max) == inRange;
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
    try {
      objects.erase(objects.begin() + index);
    } catch(std::out_of_range) {
      std::cerr << "remove: Invalid Index\n";
    }
    return *this;
  }

  ObjContainer &ObjContainer::remove(int start, int end) {
    try {
      objects.erase(objects.begin() + start, objects.begin() + end);
    } catch(std::out_of_range) {
      std::cerr << "remove: Invalid Indexes\n";
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
    for(visionObj &obj : objects) obj.print();
  }


  void ObjContainer::checkErrSig() {
    for(visionObj &obj : objects) {
      assert(obj.sig != VISION_OBJECT_ERR_SIG);
    }
  }


  ObjContainer ObjContainer::operator+(ObjContainer rhs) {
    addObj(rhs.objects);
    return *this;
  }

}