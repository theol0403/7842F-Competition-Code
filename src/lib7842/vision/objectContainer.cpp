#include "objectContainer.hpp"

namespace lib7842
{

  ObjectContainer::ObjectContainer() {}


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
      if(getObjAttr(objAttr::objSig, obj) == sig) {
        if(index <= 0) { return obj; }
        index--;
      }
    }
    return {};
  }


  double ObjectContainer::getObjAttr(objAttr attr, visionObj obj) {
    switch (attr) {
      case objAttr::objSig: return obj.objSig; break;
      case objAttr::objX: return obj.objX; break;
      case objAttr::objY: return obj.objY; break;
      case objAttr::objWidth: return obj.objWidth; break;
      case objAttr::objHeight: return obj.objHeight; break;
      case objAttr::objArea: return obj.objArea; break;
      case objAttr::objCenterX: return obj.objCenterX; break;
      case objAttr::objCenterY: return obj.objCenterY; break;
    }
    std::cerr << "GetObjAttr: Invalid Attr\n";
  }

  double ObjectContainer::getObjAttr(objAttr attr, int index) {
    return getObjAttr(attr, objects.at(index));
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
      if(getObjAttr(attr, *it) == val) {
        objects.erase(it);
      }
    }
  }

  void ObjectContainer::removeObjWithout(objAttr attr, double val) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(getObjAttr(attr, *it) != val) {
        objects.erase(it);
      }
    }
  }

  void ObjectContainer::removeObjWith(objAttr attr, double min, double max) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(min < getObjAttr(attr, *it) || getObjAttr(attr, *it) > max) {
        objects.erase(it);
      }
    }
  }

  void ObjectContainer::removeObjWithout(objAttr attr, double min, double max) {
    for(std::vector<visionObj>::iterator it = objects.begin(); it != objects.end(); it++) {
      if(!(min < getObjAttr(attr, *it) || getObjAttr(attr, *it) > max)) {
        objects.erase(it);
      }
    }
  }






  void ObjectContainer::sortBy(objAttr sortMode, bool largeToSmall)
  {
    // Loop through each object looking to swap the largest object to the right
    // except the last one, which will already be sorted by the time we get there
    for (int startIndex = 0; startIndex < currentCount - 1; startIndex++)
    {
      int greatestIndex = startIndex; //Assume current posision to swap
      bool swapNeeded = false;
      // Loop between current and end looking for greatest object
      for (int currentIndex = startIndex + 1; currentIndex < currentCount; currentIndex++)
      {
        double firstCompare = getObjValue(sortMode, currentIndex);
        double secondCompare = getObjValue(sortMode, greatestIndex);

        bool sortFound = false;
        if(largeToSmall)
        {
          sortFound = firstCompare > secondCompare;
        }
        else
        {
          sortFound = firstCompare < secondCompare;
        }
        //If current object is greater than greatest object
        if (sortFound)
        {
          greatestIndex = currentIndex;
          swapNeeded = true;
        }
      }
      if(swapNeeded) std::swap(objectArray.at(startIndex), objectArray.at(greatestIndex)); //Swap with the greatest
    }
  }



  void ObjectContainer::debugObjects(int objectCount)
  {
    debugErrorSig();
    if(objectCount > arrayLength) objectCount = arrayLength; //Bounds Checking

    std::cout << std::endl << "Length:" << arrayLength << " | " << "Count:" << currentCount << "\n";
    for(int objectNum = 0; objectNum < objectCount; objectNum++)
    {
      std::cout << "Object:" << objectNum << " | ";
      std::cout << "Sig:" << objectArray.at(objectNum).objSig << " | ";
      std::cout << "Width:" << objectArray.at(objectNum).objWidth << " | ";
      std::cout << "Height:" << objectArray.at(objectNum).objHeight << " | ";
      std::cout << "X:" << objectArray.at(objectNum).objX << " | ";
      std::cout << "Y:" << objectArray.at(objectNum).objY << " | ";
      std::cout << "Area:" << objectArray.at(objectNum).objArea << " | ";
      std::cout << "CenterX:" << objectArray.at(objectNum).objCenterX << " | ";
      std::cout << "CenterY:" << objectArray.at(objectNum).objCenterY << " | ";
      std::cout << "Discard:" << objectArray.at(objectNum).discardObject;
      std::cout << std::endl;
    }
  }

  void ObjectContainer::debugErrorSig()
  {
    for(int objectNum = 0; objectNum < currentCount; objectNum++)
    {
      if(objectArray.at(objectNum).objSig == VISION_OBJECT_ERR_SIG)
      {
        std::cout << "ERROR: ERR_SIG IN OBJECT " << objectNum << "\n";
      }
    }
  }











  void ObjectContainer::copyObjects(lib7842::ObjectContainer& sourceContainer, lib7842::ObjectContainer& destContainer, int copyNum)
  {
    int transferCount = 0;

    if(copyNum < 0)
    {
      transferCount = sourceContainer.currentCount > destContainer.arrayLength ? destContainer.arrayLength : sourceContainer.currentCount;
    }
    else
    {
      transferCount = copyNum > destContainer.arrayLength ? destContainer.arrayLength : copyNum;
    }

    for(int objectNum = 0; objectNum < transferCount; objectNum++)
    {
      destContainer.objectArray.at(objectNum) = sourceContainer.objectArray.at(objectNum);
    }
    destContainer.currentCount = transferCount;
  }




}
