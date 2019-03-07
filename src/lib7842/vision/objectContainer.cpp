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
    }
  }

  ObjectContainer::visionObj ObjectContainer::getObjBySigIndex(int, int) {
    
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






  simpleObjects_t ObjectContainer::getObject(int sizeNum, int wantedSig)
  {
    int searchIndex = 0; //Variable to point at final location

    // Search for first location with proper sig to point at
    bool sigFound = false; //Exit looping once first point found
    for(int objectNum = 0; objectNum < currentCount && !sigFound; objectNum++)
    {
      if(objectArray.at(objectNum).objSig == wantedSig)
      {
        searchIndex = objectNum;
        sigFound = true;
      }
    }

    // Skip x (sizeNum) points until match found with sizeNum 0
    bool matchFound = false; //Exit looping once final point has been found
    for(int objectNum = searchIndex; objectNum < currentCount && !matchFound; objectNum++)
    {
      if(objectArray.at(objectNum).objSig == wantedSig)
      {
        if(sizeNum <= 0)
        {
          searchIndex = objectNum;
          matchFound = true;
        }
        sizeNum--;
      }
    }

    //If either searches were not able to find something, return an object
    if(!sigFound || !matchFound)
    {
      return emptyObject;
    }

    return objectArray.at(searchIndex);
  }




  void ObjectContainer::discardObjects()
  {
    int destNum = 0;

    for (int objectNum = 0; objectNum < currentCount; objectNum++)
    {
      if(objectArray.at(objectNum).objSig != VISION_OBJECT_ERR_SIG && !objectArray.at(objectNum).discardObject)
      {
        objectArray.at(destNum).objSig = objectArray.at(objectNum).objSig;
        objectArray.at(destNum).objX = objectArray.at(objectNum).objX;
        objectArray.at(destNum).objY = objectArray.at(objectNum).objY;
        objectArray.at(destNum).objWidth = objectArray.at(objectNum).objWidth;
        objectArray.at(destNum).objHeight = objectArray.at(objectNum).objHeight;
        objectArray.at(destNum).objArea = objectArray.at(objectNum).objArea;
        objectArray.at(destNum).objCenterX = objectArray.at(objectNum).objCenterX;
        objectArray.at(destNum).objCenterY = objectArray.at(objectNum).objCenterY;
        objectArray.at(destNum).discardObject = false;
        destNum++;
      }
    }
    // for(int objectNum = destNum; objectNum < arrayLength; objectNum++) //Cleans the rest of the objects
    // {
    //   resetObject(objectNum);
    // }
    currentCount = destNum;
    debugErrorSig();
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


  void ObjectContainer::removeRange(objAttr objMode, double lowRange, double highRange, bool discard)
  {
    // loop through objects, look for size, and mark to discard
    for (int objectNum = 0; objectNum < currentCount; objectNum++)
    {
      double value = getObjValue(objMode, objectNum);

      if(value >= lowRange && value <= highRange)
      {
        objectArray.at(objectNum).discardObject = true;
      }
    }
    if(discard)
    {
      discardObjects();
    }
  }

  void ObjectContainer::removeWith(objAttr objMode, double removeNum, bool discard)
  {
    // loop through objects, look for size, and mark to discard
    for (int objectNum = 0; objectNum < currentCount; objectNum++)
    {
      double value = getObjValue(objMode, objectNum);

      if(value == removeNum)
      {
        objectArray.at(objectNum).discardObject = true;
      }
    }
    if(discard)
    {
      discardObjects();
    }
  }


  void ObjectContainer::shrinkTo(int newCount)
  {
    // count = count > currentCount ? currentCount : count;
    // currentCount = count;
    currentCount = newCount;
  }






  void ObjectContainer::filterAvgArea(double thresholdPercent, bool discard)
  {
    int avgCount = 0;
    double avgArea = 0;
    for(int objectNum = 0; objectNum < currentCount; objectNum++)
    {
      if(!objectArray.at(objectNum).discardObject)
      {
        avgCount++;
        avgArea += objectArray.at(objectNum).objArea;
      }
    }
    avgArea /= avgCount;

    // upper and lower ranges for size threshold
    double heightLow = avgArea - (avgArea * thresholdPercent);
    double heightHigh = avgArea + (avgArea * thresholdPercent);

    // loop through objects, look for size, and mark to discard
    for (int objectNum = 0; objectNum < currentCount; objectNum++)
    {
      if(objectArray.at(objectNum).objArea > heightHigh || objectArray.at(objectNum).objArea < heightLow)
      {
        objectArray.at(objectNum).discardObject = true;
      }
    }

    if(discard)
    {
      discardObjects();
    }
  }


  // Filters object proportions
  void ObjectContainer::filterProp(double thresholdPercent, double wantedProp, bool discard)
  {
    double objectWidth;
    double objectHeight;

    double heightLow;
    double heightHigh;

    // loop through objects, look for proportion, and fill into new array
    for (int objectNum = 0; objectNum < currentCount; objectNum++)
    {
      objectWidth = objectArray.at(objectNum).objWidth;
      objectHeight = objectArray.at(objectNum).objHeight;

      heightLow = (objectWidth * wantedProp) - (objectWidth * thresholdPercent);
      heightHigh = (objectWidth * wantedProp) + (objectWidth * thresholdPercent);

      if(objectHeight < heightLow || objectHeight > heightHigh)
      {
        objectArray.at(objectNum).discardObject = true;
      }
    }

    if(discard)
    {
      discardObjects();
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
