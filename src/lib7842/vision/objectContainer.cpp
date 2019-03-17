#include "objectContainer.hpp"

namespace lib7842
{

  void ObjectContainer::addObj(visionObj obj) {
    objects.push_back(obj);
  }

  void ObjectContainer::addObj(std::vector<visionObj> objs) {
    for(visionObj &obj : objs) addObj(obj);
  }

  visionObj ObjectContainer::get(int index) {
    try {
      return objects.at(index);
    } catch(std::out_of_range) {
      std::cerr << "get: Invalid Index\n";
      return {};
    }
  }

  visionObj ObjectContainer::getTotal() {
    return std::accumulate(objects.begin(), objects.end(), visionObj{});
  }

  visionObj ObjectContainer::getAvg() {
    return getTotal() / objects.size();
  }


  ObjectContainer::removeFunc_t ObjectContainer::makeRemove(objAttr attr, std::function<bool(double, double)> compare, double val) {
    return [=](const visionObj& obj) {
      return compare(obj.getAttr(attr), val);
    };
  }

  ObjectContainer::removeFunc_t ObjectContainer::makeRemove(objAttr attr, bool inRange, double min, double max) {
    return [=](const visionObj& obj) {
      return (min < obj.getAttr(attr) || obj.getAttr(attr) > max) == inRange;
    };
  }

  void ObjectContainer::removeBy(removeFunc_t removeFunc) {
    objects.erase(std::remove_if(objects.begin(), objects.end(), removeFunc), objects.end());
  }


  ObjectContainer::sortFunc_t ObjectContainer::makeSort(objAttr attr, bool decending) {
    return [=](const visionObj& first, const visionObj& second) {
      if(decending) {
        return first.getAttr(attr) > second.getAttr(attr);
      } else {
        return first.getAttr(attr) < second.getAttr(attr);
      }
    };
  }

  void ObjectContainer::sortBy(sortFunc_t sortFunc) {
    std::stable_sort(objects.begin(), objects.end(), sortFunc);
  }



  ObjectContainer ObjectContainer::copy() {
    return *this;
  }


  ObjectContainer &ObjectContainer::resize(int size) {
    objects.resize(size);
    return *this;
  }

  ObjectContainer &ObjectContainer::trim(int size) {
    if(size < objects.size()) resize(size);
    return *this;
  }

  ObjectContainer &ObjectContainer::remove(int index) {
    try {
      objects.erase(objects.begin() + index);
    } catch(std::out_of_range) {
      std::cerr << "remove: Invalid Index\n";
    }
    return *this;
  }

  ObjectContainer &ObjectContainer::remove(int start, int end) {
    try {
      objects.erase(objects.begin() + start, objects.begin() + end);
    } catch(std::out_of_range) {
      std::cerr << "remove: Invalid Indexes\n";
    }
    return *this;
  }


  ObjectContainer &ObjectContainer::removeBy(objAttr attr, std::function<bool(double, double)> compare, double val) {
    removeBy(makeRemove(attr, compare, val));
    return *this;
  }

  ObjectContainer &ObjectContainer::removeWith(objAttr attr, double val) {
    removeBy(makeRemove(attr, std::equal_to<>(), val));
    return *this;
  }

  ObjectContainer &ObjectContainer::removeWithout(objAttr attr, double val) {
    removeBy(makeRemove(attr, std::not_equal_to<>(), val));
    return *this;
  }

  ObjectContainer &ObjectContainer::removeWith(objAttr attr, double min, double max) {
    removeBy(makeRemove(attr, true, min, max));
    return *this;
  }

  ObjectContainer &ObjectContainer::removeWithout(objAttr attr, double min, double max) {
    removeBy(makeRemove(attr, false, min, max));
    return *this;
  }


  ObjectContainer &ObjectContainer::sortBy(objAttr attr, bool decending) {
    sortBy(makeSort(attr, decending));
    return *this;
  }


  void ObjectContainer::reset() {
    resize(0);
  }


  void ObjectContainer::print() {
    std::cout << "Count:" << objects.size() << std::endl;
    for(visionObj &obj : objects) obj.print();
  }


  void ObjectContainer::checkErrSig() {
    for(visionObj &obj : objects) {
      assert(obj.sig != VISION_OBJECT_ERR_SIG);
    }
  }


  ObjectContainer ObjectContainer::operator+(ObjectContainer rhs) {
    addObj(rhs.objects);
    return *this;
  }

}
