#include "ObjectSmoothing.hpp"


ObjectSmoothing::ObjectSmoothing(ObjectContainer& sourceContainer, ObjectContainer& destContainer, double objectPosThreshold, double emaAlpha, bool differentVel, double emaAlphaVel, int lifeMax, int lifeThreshold)
:
m_sourceContainer(&sourceContainer),
m_destContainer(&destContainer),
m_objectPosThreshold(objectPosThreshold),
m_emaAlpha(emaAlpha),
m_differentVel(differentVel),
m_emaAlphaVel(emaAlphaVel),
m_lifeMax(lifeMax),
m_lifeThreshold(lifeThreshold),
m_masterLength(m_sourceContainer->arrayLength * lifeMax)
{
  m_sourceObjects.resize(m_sourceContainer->arrayLength);
  clearArray(m_sourceObjects, 0, m_sourceContainer->arrayLength-1);

  m_masterObjects.resize(m_masterLength);
  clearArray(m_masterObjects, 0, m_masterLength-1);

  m_masterCount = 0;

}

ObjectSmoothing::~ObjectSmoothing()
{
}


void ObjectSmoothing::clearArray(std::vector<sortedObjects_t>& clearArray, int startIndex, int endIndex)
{
  for (int objectNum = startIndex; objectNum <= endIndex; objectNum++)
  {
    clearArray.at(objectNum).objSig = VISION_OBJECT_ERR_SIG;
    clearArray.at(objectNum).objY = 0;
    clearArray.at(objectNum).objX = 0;
    clearArray.at(objectNum).objWidth = 0;
    clearArray.at(objectNum).objHeight = 0;
    clearArray.at(objectNum).objCenterX = 0;
    clearArray.at(objectNum).objCenterY = 0;
    clearArray.at(objectNum).objXVel = 0;
    clearArray.at(objectNum).objYVel = 0;

    clearArray.at(objectNum).matchFound = false;
    clearArray.at(objectNum).lifeCounter = 0;
  }
}


void ObjectSmoothing::swapObjects(std::vector<sortedObjects_t>& swapArray, int firstIndex, int secondIndex)
{
  sortedObjects_t tempObject = {};
  tempObject.objSig = swapArray.at(firstIndex).objSig;
  tempObject.objX = swapArray.at(firstIndex).objX;
  tempObject.objY = swapArray.at(firstIndex).objY;
  tempObject.objWidth = swapArray.at(firstIndex).objWidth;
  tempObject.objHeight = swapArray.at(firstIndex).objHeight;
  tempObject.objCenterX = swapArray.at(firstIndex).objCenterX;
  tempObject.objCenterY = swapArray.at(firstIndex).objCenterY;
  tempObject.objXVel = swapArray.at(firstIndex).objXVel;
  tempObject.objYVel = swapArray.at(firstIndex).objYVel;
  tempObject.matchFound = swapArray.at(firstIndex).matchFound;
  tempObject.lifeCounter = swapArray.at(firstIndex).lifeCounter;

  swapArray.at(firstIndex).objSig = swapArray.at(secondIndex).objSig;
  swapArray.at(firstIndex).objX = swapArray.at(secondIndex).objX;
  swapArray.at(firstIndex).objY = swapArray.at(secondIndex).objY;
  swapArray.at(firstIndex).objWidth = swapArray.at(secondIndex).objWidth;
  swapArray.at(firstIndex).objHeight = swapArray.at(secondIndex).objHeight;
  swapArray.at(firstIndex).objCenterX = swapArray.at(secondIndex).objCenterX;
  swapArray.at(firstIndex).objCenterY = swapArray.at(secondIndex).objCenterY;
  swapArray.at(firstIndex).objXVel = swapArray.at(secondIndex).objXVel;
  swapArray.at(firstIndex).objYVel = swapArray.at(secondIndex).objYVel;
  swapArray.at(firstIndex).matchFound = swapArray.at(secondIndex).matchFound;
  swapArray.at(firstIndex).lifeCounter = swapArray.at(secondIndex).lifeCounter;

  swapArray.at(secondIndex).objSig = tempObject.objSig;
  swapArray.at(secondIndex).objX = tempObject.objX;
  swapArray.at(secondIndex).objY = tempObject.objY;
  swapArray.at(secondIndex).objWidth = tempObject.objWidth;
  swapArray.at(secondIndex).objHeight = tempObject.objHeight;
  swapArray.at(secondIndex).objCenterX = tempObject.objCenterX;
  swapArray.at(secondIndex).objCenterY = tempObject.objCenterY;
  swapArray.at(secondIndex).objXVel = tempObject.objXVel;
  swapArray.at(secondIndex).objYVel = tempObject.objYVel;
  swapArray.at(secondIndex).matchFound = tempObject.matchFound;
  swapArray.at(secondIndex).lifeCounter = tempObject.lifeCounter;
}


void ObjectSmoothing::sortArrayY(std::vector<sortedObjects_t>& sortArray, int firstIndex, int lastIndex)
{
  // Loop through each object looking to swap the largest object to the right
  // except the last one, which will already be sorted by the time we get there
  for (int startIndex = firstIndex; startIndex < lastIndex; startIndex++)
  {
    int greatestIndex = startIndex; //Assume current posision to swap
    bool swapNeeded = false;
    // Loop between current and end looking for greatest object
    for (int currentIndex = startIndex + 1; currentIndex <= lastIndex; currentIndex++)
    {
      //If current object is greater than greatest object
      if (sortArray.at(currentIndex).objCenterY > sortArray.at(greatestIndex).objCenterY)
      {
        greatestIndex = currentIndex;
        swapNeeded = true;
      }
    }
    if(swapNeeded) swapObjects(sortArray, startIndex, greatestIndex); //Swap with the greatest
  }
}

void ObjectSmoothing::sortArrayLife(std::vector<sortedObjects_t>& sortArray, int firstIndex, int lastIndex)
{
  // Loop through each object looking to swap the largest object to the right
  // except the last one, which will already be sorted by the time we get there
  int swapCount = 0;
  for (int startIndex = firstIndex; startIndex < lastIndex; startIndex++)
  {
    int greatestIndex = startIndex; //Assume current posision to swap
    bool swapNeeded = false;
    // Loop between current and end looking for greatest object
    if(sortArray.at(startIndex).lifeCounter < m_lifeMax) //Only swap if nessesary
    {
      for (int currentIndex = startIndex + 1; currentIndex <= lastIndex; currentIndex++)
      {
        //If current object is greater than greatest object
        if (sortArray.at(currentIndex).lifeCounter > sortArray.at(greatestIndex).lifeCounter)
        {
          greatestIndex = currentIndex;
          swapNeeded = true;
        }
      }
      if(swapNeeded)
      {
        swapCount++;
        swapObjects(sortArray, startIndex, greatestIndex); //Swap with the greatest
      }
    }
  }
  // if(m_debugInfo)
  // {
  //   std::cout << " | " << "L Swap: " << swapCount;
  // }
}


//Imports the source array and sorts it by Y into sourceObjects
void ObjectSmoothing::importSource()
{
  //Copies source into m_source array
  for (int objectNum = 0; objectNum < m_sourceContainer->currentCount; objectNum++)
  {
    if(m_sourceContainer->objectArray.at(objectNum).objSig != VISION_OBJECT_ERR_SIG)
    {
      m_sourceObjects.at(objectNum).objSig = m_sourceContainer->objectArray.at(objectNum).objSig;
      m_sourceObjects.at(objectNum).objX = m_sourceContainer->objectArray.at(objectNum).objX;
      m_sourceObjects.at(objectNum).objY = m_sourceContainer->objectArray.at(objectNum).objY;
      m_sourceObjects.at(objectNum).objWidth = m_sourceContainer->objectArray.at(objectNum).objWidth;
      m_sourceObjects.at(objectNum).objHeight = m_sourceContainer->objectArray.at(objectNum).objHeight;
      m_sourceObjects.at(objectNum).objCenterX = m_sourceContainer->objectArray.at(objectNum).objCenterX;
      m_sourceObjects.at(objectNum).objCenterY = m_sourceContainer->objectArray.at(objectNum).objCenterY;
      m_sourceObjects.at(objectNum).matchFound = false;
      m_sourceObjects.at(objectNum).lifeCounter = 0;
    }
    else std::cout << "Invalid Source: Err" << objectNum << "\n";
  }
  //Resets the remaining slots in the m_sourceObjects
  clearArray(m_sourceObjects, m_sourceContainer->currentCount, m_sourceContainer->arrayLength-1);
  sortArrayY(m_sourceObjects, 0, m_sourceContainer->currentCount-1); //Sorts m_sourceObjects by Y
}


bool ObjectSmoothing::compareObjects(sortedObjects_t* masterObject, sortedObjects_t* sourceObject)
{
  //compare sig, then Y, then X, then size
  bool cascadingTrue = true;
  cascadingTrue = cascadingTrue && sourceObject->objSig == masterObject->objSig;
  //std::cout << "Y Increment " << sourceObject->objCenterY - masterObject->objCenterY<< "\n";
  cascadingTrue = cascadingTrue && (sourceObject->objCenterX > masterObject->objCenterX - m_objectPosThreshold) && (sourceObject->objCenterX < masterObject->objCenterX + m_objectPosThreshold);
  cascadingTrue = cascadingTrue && (sourceObject->objCenterY > masterObject->objCenterY - m_objectPosThreshold) && (sourceObject->objCenterY < masterObject->objCenterY + m_objectPosThreshold);
  cascadingTrue = cascadingTrue && (sourceObject->objWidth > masterObject->objWidth - m_objectPosThreshold/2) && (sourceObject->objWidth < masterObject->objWidth + m_objectPosThreshold/2);
  cascadingTrue = cascadingTrue && (sourceObject->objHeight > masterObject->objHeight - m_objectPosThreshold/2) && (sourceObject->objHeight < masterObject->objHeight + m_objectPosThreshold/2);
  return cascadingTrue;
}


double ObjectSmoothing::emaCalculate(double lastValue, double newValue, double emaAlpha)
{
  return (emaAlpha * newValue + (1.0 - emaAlpha) * lastValue);
}

void ObjectSmoothing::mergeObject(sortedObjects_t &destObject, sortedObjects_t &newObject)
{
  destObject.objSig = newObject.objSig;
  destObject.objWidth = emaCalculate(destObject.objWidth, newObject.objWidth, m_emaAlpha);
  destObject.objHeight = emaCalculate(destObject.objHeight, newObject.objHeight, m_emaAlpha);

  if(m_differentVel)
  {
    destObject.objX = emaCalculate(destObject.objX, newObject.objX + destObject.objXVel/m_emaAlphaVel, m_emaAlpha);
    destObject.objY = emaCalculate(destObject.objY, newObject.objY + destObject.objYVel/m_emaAlphaVel, m_emaAlpha);
    destObject.objCenterX = emaCalculate(destObject.objCenterX, newObject.objCenterX + destObject.objXVel/m_emaAlphaVel, m_emaAlpha);
    destObject.objCenterY = emaCalculate(destObject.objCenterY, newObject.objCenterY + destObject.objYVel/m_emaAlphaVel, m_emaAlpha);
  }
  else
  {
    destObject.objX = emaCalculate(destObject.objX, newObject.objX, m_emaAlpha) + destObject.objXVel;
    destObject.objY = emaCalculate(destObject.objY, newObject.objY, m_emaAlpha) + destObject.objYVel;
    destObject.objCenterX = emaCalculate(destObject.objCenterX, newObject.objCenterX, m_emaAlpha) + destObject.objXVel;
    destObject.objCenterY = emaCalculate(destObject.objCenterY, newObject.objCenterY, m_emaAlpha) + destObject.objYVel;
  }


  destObject.objXVel = emaCalculate(destObject.objXVel, (newObject.objCenterX - destObject.objCenterX), m_emaAlphaVel);
  destObject.objYVel = emaCalculate(destObject.objYVel, (newObject.objCenterY - destObject.objCenterY), m_emaAlphaVel);
  //std::cout << "XVel:" << destObject.objXVel << "\n";

  destObject.lifeCounter+=5; //Increase life
  if(destObject.lifeCounter > m_lifeMax) destObject.lifeCounter = m_lifeMax;
}

void ObjectSmoothing::pushObject(sortedObjects_t &destObject, sortedObjects_t &newObject)
{
  destObject.objSig = newObject.objSig;
  destObject.objX = newObject.objX;
  destObject.objY = newObject.objY;
  destObject.objWidth = newObject.objWidth;
  destObject.objHeight = newObject.objHeight;
  destObject.objCenterX = newObject.objCenterX;
  destObject.objCenterY = newObject.objCenterY;
  destObject.objXVel = 0;
  destObject.objYVel = 0;
  destObject.lifeCounter = 1;
}

void ObjectSmoothing::trimObject(sortedObjects_t &destObject)
{
  if(m_differentVel)
  {
    destObject.objX += destObject.objXVel;
    destObject.objY += destObject.objYVel;
    destObject.objCenterX += destObject.objXVel;
    destObject.objCenterY += destObject.objYVel;
  }
  else
  {
    destObject.objX += destObject.objXVel;
    destObject.objY += destObject.objYVel;
    destObject.objCenterX += destObject.objXVel;
    destObject.objCenterY += destObject.objYVel;
  }

  destObject.lifeCounter--;
}


void ObjectSmoothing::mergeObjects()
{
  int mergeCount = 0;
  int pushCount = 0;
  int trimCount = 0;

  int newMasterCount = m_masterCount;
  bool sourceMatchFound = false;
  //for every source object
  for(int sourceObjectNum = 0; sourceObjectNum < m_sourceContainer->currentCount; sourceObjectNum++)
  {
    sourceMatchFound = false;
    //loop through all current master objects
    for(int masterNum = 0; masterNum < m_masterCount && !sourceMatchFound; masterNum++)
    {
      if(!m_masterObjects.at(masterNum).matchFound)
      {
        if(compareObjects(&m_masterObjects.at(masterNum), &m_sourceObjects[sourceObjectNum]))
        {
          mergeCount++;
          mergeObject(m_masterObjects.at(masterNum), m_sourceObjects[sourceObjectNum]);
          m_masterObjects.at(masterNum).matchFound = true;
          sourceMatchFound = true;
        }
      }
    }
    //If no match was found, if all matches have already been made, or if there are more sources than master
    if(!sourceMatchFound)
    {
      pushCount++;
      //Add extra object to the end of master
      pushObject(m_masterObjects.at(m_masterCount), m_sourceObjects[sourceObjectNum]);
      m_masterObjects.at(m_masterCount).matchFound = true;
      newMasterCount++; //Let merge know there are extra objects
    }
  }
  for(int masterNum = 0; masterNum < m_masterCount; masterNum++) //Scan through master looking for non-matches
  {
    if(!m_masterObjects.at(masterNum).matchFound)
    {
      trimCount++;
      trimObject(m_masterObjects.at(masterNum));
    }
  }

  m_masterCount = newMasterCount;

  std::cout << std::endl << "Count:" << m_masterCount << " | " << "M:" << mergeCount << " " << "P:" << pushCount << " " << "T:" << trimCount;

}


void ObjectSmoothing::sortMaster()
{
  //Reset master match found bools
  //delete all elements with a life of 0 and move array down
  //update masterCount to reflect the deleted objects
  //Sort array by life
  //sort array by y

  for(int masterNum = 0; masterNum < m_masterLength; masterNum++) //Clear match bool
  {
    m_masterObjects.at(masterNum).matchFound = false;
  }

  int purgeCount = 0;

  int newMasterCount = m_masterCount;
  bool endScan = false;
  for(int masterNum = 0; masterNum < m_masterCount && !endScan; masterNum++) //Clear match bool
  {
    if(m_masterObjects.at(masterNum).lifeCounter <= 0)
    {
      purgeCount++;
      newMasterCount--;
    }
  }

    std::cout << " | " << "R:" << purgeCount;

  sortArrayLife(m_masterObjects, 0, m_masterCount); //Moves all 0 objects to the right
  m_masterCount = newMasterCount;

  int startIndex = 0;
  for(int lifeCount = m_lifeMax; (lifeCount > m_lifeMax - m_lifeThreshold) && (startIndex < m_masterCount); lifeCount--)
  {
    int searchBoundary = startIndex;
    bool sortNeeded = false;
    while(m_masterObjects.at(searchBoundary).lifeCounter >= lifeCount && searchBoundary < m_masterCount)
    {
      sortNeeded = true;
      searchBoundary++;
    }
    if(sortNeeded && startIndex != searchBoundary-1)
    {

        std::cout << " | Y Sort: " << startIndex << "-" << searchBoundary-1;
      sortArrayY(m_masterObjects, startIndex, searchBoundary-1);
    }
    startIndex = searchBoundary;
  }

  clearArray(m_masterObjects, m_masterCount, m_masterLength-1);
}


void ObjectSmoothing::exportObjects(int minLife, int maxLife)
{
  int exportCount = 0;
  //for each object in master down to x life
  for(int objectNum = 0; objectNum < m_destContainer->arrayLength; objectNum++)
  {
    if(m_masterObjects.at(objectNum).lifeCounter <= maxLife && m_masterObjects.at(objectNum).lifeCounter >= minLife)
    {
      m_destContainer->objectArray.at(objectNum).objSig = m_masterObjects.at(objectNum).objSig;
      m_destContainer->objectArray.at(objectNum).objX = m_masterObjects.at(objectNum).objX;
      m_destContainer->objectArray.at(objectNum).objY = m_masterObjects.at(objectNum).objY;
      m_destContainer->objectArray.at(objectNum).objWidth = m_masterObjects.at(objectNum).objWidth;
      m_destContainer->objectArray.at(objectNum).objHeight = m_masterObjects.at(objectNum).objHeight;
      m_destContainer->objectArray.at(objectNum).objSize = (m_masterObjects.at(objectNum).objWidth + m_masterObjects.at(objectNum).objHeight) / 2;
      m_destContainer->objectArray.at(objectNum).objCenterX = m_masterObjects.at(objectNum).objCenterX;
      m_destContainer->objectArray.at(objectNum).objCenterY = m_masterObjects.at(objectNum).objCenterY;
      m_destContainer->objectArray.at(objectNum).discardObject = false;
      exportCount = objectNum + 1;
    }
    else
    {
      m_destContainer->objectArray.at(objectNum).objSig = VISION_OBJECT_ERR_SIG;
      m_destContainer->objectArray.at(objectNum).objX = 0;
      m_destContainer->objectArray.at(objectNum).objY = 0;
      m_destContainer->objectArray.at(objectNum).objWidth = 0;
      m_destContainer->objectArray.at(objectNum).objHeight = 0;
      m_destContainer->objectArray.at(objectNum).objSize = 0;
      m_destContainer->objectArray.at(objectNum).objCenterX = 0;
      m_destContainer->objectArray.at(objectNum).objCenterY = 0;
      m_destContainer->objectArray.at(objectNum).discardObject = false;
    }
  }
  m_destContainer->currentCount = exportCount;
}



void ObjectSmoothing::smoothObjects()
{
  importSource();
  mergeObjects();
  sortMaster();
  exportObjects(m_lifeMax - m_lifeThreshold, m_lifeMax);
}
