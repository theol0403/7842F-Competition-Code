#include "objectContainer.hpp"

namespace lib7842
{
  const simpleObjects_t ObjectContainer::emptyObject;

  ObjectContainer::ObjectContainer(int objectCount)
  : arrayLength(objectCount)
  {
    objectArray.resize(arrayLength);
  }

  ObjectContainer::ObjectContainer(int objectCount, ObjectDrawing& screenParent)
  :
  arrayLength(objectCount)
  {
    objectArray.resize(arrayLength);

    screenArray.resize(arrayLength);
    for(int objectNum = 0; objectNum < arrayLength; objectNum++)
    {
      screenArray.at(objectNum) = lv_obj_create(screenParent.m_drawingContainer, NULL);
      lv_obj_set_style(screenArray.at(objectNum), &screenParent.m_defaultObjectStyle);
      lv_obj_set_hidden(screenArray.at(objectNum), true);
    }

    for (lv_style_t &sigStyle : styleArray)
    {
      lv_style_copy(&sigStyle, &screenParent.m_defaultObjectStyle);
    }
  }

  ObjectContainer::~ObjectContainer() {} //Delete stuff TODO


  void ObjectContainer::setContainerStyle(lv_color_t bodyColor, lv_color_t borderColor, lv_opa_t opaNum)
  {
    for (lv_style_t &sigStyle : styleArray)
    {
      sigStyle.body.main_color = bodyColor;
      sigStyle.body.grad_color = bodyColor;
      sigStyle.body.border.color = borderColor;
      sigStyle.body.opa = opaNum;
    }
  }

  void ObjectContainer::setSigStyle(int sigNum, lv_color_t bodyColor, lv_color_t borderColor, lv_opa_t opaNum)
  {
    styleArray.at(sigNum).body.main_color = bodyColor;
    styleArray.at(sigNum).body.grad_color = bodyColor;
    styleArray.at(sigNum).body.border.color = borderColor;
    styleArray.at(sigNum).body.opa = opaNum;
  }



  void ObjectContainer::sortBySize()
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
        //If current object is greater than greatest object
        if (objectArray.at(currentIndex).objArea > objectArray.at(greatestIndex).objArea)
        {
          greatestIndex = currentIndex;
          swapNeeded = true;
        }
      }
      if(swapNeeded) std::swap(objectArray.at(startIndex), objectArray.at(greatestIndex)); //Swap with the greatest
    }
  }


  simpleObjects_t ObjectContainer::getObject(int sizeNum)
  {
    if(sizeNum < currentCount)
    {
      return objectArray.at(sizeNum);
    }
    else
    {
      return emptyObject;
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



  void ObjectContainer::filterAreaRange(double lowArea, double highArea, bool discard)
  {
    // loop through objects, look for size, and mark to discard
    for (int objectNum = 0; objectNum < currentCount; objectNum++)
    {
      if(objectArray.at(objectNum).objArea >= lowArea && objectArray.at(objectNum).objArea <= highArea)
      {
        objectArray.at(objectNum).discardObject = true;
      }
    }
    if(discard)
    {
      discardObjects();
    }
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


  void ObjectContainer::discardObjects()
  {
    int destNum = 0;

    for (int objectNum = 0; objectNum < currentCount; objectNum++)
    {
      if(objectArray.at(objectNum).objSig != VISION_OBJECT_ERR_SIG && !objectArray.at(destNum).discardObject)
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


  void ObjectContainer::debugObjects(int objectCount)
  {
    debugErrorSig();
    if(objectCount > arrayLength) objectCount = arrayLength; //Bounds Checking

    std::cout << "Length:" << arrayLength << " | " << "Count:" << currentCount << "\n";
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

}