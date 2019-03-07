#include "objectContainer.hpp"

namespace lib7842
{

  ObjectContainer::visionObj ObjectContainer::visionObj::operator+(ObjectContainer::visionObj rhs){
    objX = objX + rhs.objX;
    objY = objY + rhs.objY;
    objWidth = objWidth + rhs.objWidth;
    objHeight = objHeight + rhs.objHeight;
    objArea = objArea + rhs.objArea;
    objCenterX = objCenterX + rhs.objCenterX;
    objCenterY = objCenterY + rhs.objCenterY;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator-(ObjectContainer::visionObj rhs){
    objX = objX - rhs.objX;
    objY = objY - rhs.objY;
    objWidth = objWidth - rhs.objWidth;
    objHeight = objHeight - rhs.objHeight;
    objArea = objArea + rhs.objArea;
    objCenterX = objCenterX - rhs.objCenterX;
    objCenterY = objCenterY - rhs.objCenterY;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator*(ObjectContainer::visionObj rhs){
    objX = objX * rhs.objX;
    objY = objY * rhs.objY;
    objWidth = objWidth * rhs.objWidth;
    objHeight = objHeight * rhs.objHeight;
    objArea = objArea * rhs.objArea;
    objCenterX = objCenterX * rhs.objCenterX;
    objCenterY = objCenterY * rhs.objCenterY;
    return *this;
  }

  ObjectContainer::visionObj ObjectContainer::visionObj::operator/(ObjectContainer::visionObj rhs){
    objX = objX / rhs.objX;
    objY = objY / rhs.objY;
    objWidth = objWidth / rhs.objWidth;
    objHeight = objHeight / rhs.objHeight;
    objArea = objArea / rhs.objArea;
    objCenterX = objCenterX / rhs.objCenterX;
    objCenterY = objCenterY / rhs.objCenterY;
    return *this;
  }

  double ObjectContainer::visionObj::getAttr(objAttr attr) const {
    switch (attr) {
      case objAttr::objSig: return objSig; break;
      case objAttr::objX: return objX; break;
      case objAttr::objY: return objY; break;
      case objAttr::objWidth: return objWidth; break;
      case objAttr::objHeight: return objHeight; break;
      case objAttr::objArea: return objArea; break;
      case objAttr::objCenterX: return objCenterX; break;
      case objAttr::objCenterY: return objCenterY; break;
    }
    std::cerr << "GetAttr: Invalid Attr\n";
  }


  void ObjectContainer::resize(int size) {
    objects.resize(size);
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

  ObjectContainer::visionObj ObjectContainer::getObjByIndex(int index) {
    try {
      return objects.at(index);
    } catch(std::out_of_range) {
      std::cerr << "GetObjByIndex: Invalid Index\n";
      return {};
    }
  }

  ObjectContainer::visionObj ObjectContainer::getObjBySigIndex(int sig, int index) {
    for(visionObj &obj : objects) {
      if(obj.getAttr(objAttr::objSig) == sig) {
        if(index <= 0) { return obj; }
        index--;
      }
    }
    return {};
  }


  double ObjectContainer::getObjAttr(objAttr attr, int index) {
    return objects.at(index).getAttr(attr);
  }

  double ObjectContainer::getTotalAttr(objAttr attr) {
    return std::accumulate(objects.begin(), objects.end(), visionObj{}).getAttr(attr);
  }

  double ObjectContainer::getAvgAttr(objAttr attr) {
    return getTotalAttr(attr) / objects.size();
  }



  void ObjectContainer::removeObjIndex(int index) {
    try {
      objects.erase(objects.begin() + index);
    } catch(std::out_of_range) {
      std::cerr << "RemoveObjIndex: Invalid Index\n";
    }
  }

  void ObjectContainer::removeObjWith(objAttr attr, double val) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(it->getAttr(attr) == val) { objects.erase(it); }
    }
  }

  void ObjectContainer::removeObjWithout(objAttr attr, double val) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(it->getAttr(attr) != val) { objects.erase(it); }
    }
  }

  void ObjectContainer::removeObjWith(objAttr attr, double min, double max) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(min < it->getAttr(attr) || it->getAttr(attr) > max) { objects.erase(it); }
    }
  }

  void ObjectContainer::removeObjWithout(objAttr attr, double min, double max) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(!(min < it->getAttr(attr) || it->getAttr(attr) > max)) { objects.erase(it); }
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







  // void ObjectContainer::debugObjects(int objectCount)
  // {
  //   debugErrorSig();
  //   if(objectCount > arrayLength) objectCount = arrayLength; //Bounds Checking
  //
  //   std::cout << std::endl << "Length:" << arrayLength << " | " << "Count:" << currentCount << "\n";
  //   for(int objectNum = 0; objectNum < objectCount; objectNum++)
  //   {
  //     std::cout << "Object:" << objectNum << " | ";
  //     std::cout << "Sig:" << objectArray.at(objectNum).objSig << " | ";
  //     std::cout << "Width:" << objectArray.at(objectNum).objWidth << " | ";
  //     std::cout << "Height:" << objectArray.at(objectNum).objHeight << " | ";
  //     std::cout << "X:" << objectArray.at(objectNum).objX << " | ";
  //     std::cout << "Y:" << objectArray.at(objectNum).objY << " | ";
  //     std::cout << "Area:" << objectArray.at(objectNum).objArea << " | ";
  //     std::cout << "CenterX:" << objectArray.at(objectNum).objCenterX << " | ";
  //     std::cout << "CenterY:" << objectArray.at(objectNum).objCenterY << " | ";
  //     std::cout << "Discard:" << objectArray.at(objectNum).discardObject;
  //     std::cout << std::endl;
  //   }
  // }
  //
  // void ObjectContainer::debugErrorSig()
  // {
  //   for(int objectNum = 0; objectNum < currentCount; objectNum++)
  //   {
  //     if(objectArray.at(objectNum).objSig == VISION_OBJECT_ERR_SIG)
  //     {
  //       std::cout << "ERROR: ERR_SIG IN OBJECT " << objectNum << "\n";
  //     }
  //   }
  // }
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  // void ObjectContainer::copyObjects(lib7842::ObjectContainer& sourceContainer, lib7842::ObjectContainer& destContainer, int copyNum)
  // {
  //   int transferCount = 0;
  //
  //   if(copyNum < 0)
  //   {
  //     transferCount = sourceContainer.currentCount > destContainer.arrayLength ? destContainer.arrayLength : sourceContainer.currentCount;
  //   }
  //   else
  //   {
  //     transferCount = copyNum > destContainer.arrayLength ? destContainer.arrayLength : copyNum;
  //   }
  //
  //   for(int objectNum = 0; objectNum < transferCount; objectNum++)
  //   {
  //     destContainer.objectArray.at(objectNum) = sourceContainer.objectArray.at(objectNum);
  //   }
  //   destContainer.currentCount = transferCount;
  // }




}
