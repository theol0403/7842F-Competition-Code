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

  ObjectContainer &ObjectContainer::removeWith(objAttr attr, double val) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(it->getAttr(attr) == val) objects.erase(it);
    }
    return *this;
  }

  ObjectContainer &ObjectContainer::removeWithout(objAttr attr, double val) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(it->getAttr(attr) != val) objects.erase(it);
    }
    return *this;
  }

  ObjectContainer &ObjectContainer::removeWith(objAttr attr, double min, double max) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(min < it->getAttr(attr) || it->getAttr(attr) > max) objects.erase(it);
    }
    return *this;
  }

  ObjectContainer &ObjectContainer::removeWithout(objAttr attr, double min, double max) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(!(min < it->getAttr(attr) || it->getAttr(attr) > max)) objects.erase(it);
    }
    return *this;
  }

  ObjectContainer &ObjectContainer::sortBy(objAttr attr, bool decending) {
    sortBy(makeSort(attr, decending));
    return *this;
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


  void ObjectContainer::reset() {
    resize(0);
  }


  void ObjectContainer::print() {
    std::cout << "Count:" << objects.size() << std::endl;
    for(visionObj &obj : objects) obj.print();
  }


  void ObjectContainer::checkErrSig() {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(it->objSig == VISION_OBJECT_ERR_SIG) {
        std::cout << "checkErrSig: ERR_SIG in object" << it - objects.begin() << std::endl;
      }
    }
  }


  ObjectContainer ObjectContainer::operator+(ObjectContainer rhs) {
    addObj(rhs.objects);
    return *this;
  }

}
