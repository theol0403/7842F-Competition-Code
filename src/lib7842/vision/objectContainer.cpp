#include "objectContainer.hpp"

namespace lib7842
{

  void ObjectContainer::resize(int size) {
    objects.resize(size);
  }

  void ObjectContainer::shrinkTo(int size) {
    if(size < objects.size()) resize(size);
  }

  void ObjectContainer::reset() {
    resize(0);
  }

  void ObjectContainer::addObj(visionObj obj) {
    objects.push_back(obj);
  }

  void ObjectContainer::addObj(std::vector<visionObj> objs) {
    for(visionObj &obj : objs) addObj(obj);
  }

  visionObj ObjectContainer::getObjByIndex(int index) {
    try {
      return objects.at(index);
    } catch(std::out_of_range) {
      std::cerr << "getObjByIndex: Invalid Index\n";
      return {};
    }
  }

  visionObj ObjectContainer::getObjBySigIndex(int sig, int index) {
    for(visionObj &obj : objects) {
      if(obj.getAttr(objAttr::objSig) == sig) {
        if(index <= 0) return obj;
        index--;
      }
    }
    return {};
  }

  visionObj ObjectContainer::getTotalObj() {
    return std::accumulate(objects.begin(), objects.end(), visionObj{});
  }

  visionObj ObjectContainer::getAvgObj() {
    return getTotalObj() / objects.size();
  }

  double ObjectContainer::getObjAttr(objAttr attr, int index) {
    return objects.at(index).getAttr(attr);
  }

  double ObjectContainer::getTotalAttr(objAttr attr) {
    return getTotalObj().getAttr(attr);
  }

  double ObjectContainer::getAvgAttr(objAttr attr) {
    return getAvgObj().getAttr(attr);
  }


  void ObjectContainer::removeObjIndex(int index) {
    try {
      objects.erase(objects.begin() + index);
    } catch(std::out_of_range) {
      std::cerr << "removeObjIndex: Invalid Index\n";
    }
  }

  void ObjectContainer::removeObjWith(objAttr attr, double val) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(it->getAttr(attr) == val) objects.erase(it);
    }
  }

  void ObjectContainer::removeObjWithout(objAttr attr, double val) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(it->getAttr(attr) != val) objects.erase(it);
    }
  }

  void ObjectContainer::removeObjWith(objAttr attr, double min, double max) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(min < it->getAttr(attr) || it->getAttr(attr) > max) objects.erase(it);
    }
  }

  void ObjectContainer::removeObjWithout(objAttr attr, double min, double max) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(!(min < it->getAttr(attr) || it->getAttr(attr) > max)) objects.erase(it);
    }
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

  void ObjectContainer::sortBy(objAttr attr, bool decending) {
    sortBy(makeSort(attr, decending));
  }

  void ObjectContainer::sortBy(sortFunc_t sortFunc) {
    std::stable_sort(objects.begin(), objects.end(), sortFunc);
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
