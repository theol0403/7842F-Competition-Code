#include "ObjectSmoothing.hpp"


ObjectSmoothing::ObjectSmoothing(int objectCount, double objectPosThreshold, double emaAlpha, bool differentVel, double emaAlphaVel, int lifeMax, int lifeThreshold, bool debugInfo)
:
m_sourceLength(objectCount),
m_objectPosThreshold(objectPosThreshold),
m_emaAlpha(emaAlpha),
m_differentVel(differentVel),
m_emaAlphaVel(emaAlphaVel),
m_lifeMax(lifeMax),
m_lifeThreshold(lifeThreshold),
m_masterLength(objectCount * lifeMax),
m_debugInfo(debugInfo)
{
  m_sourceObjects = new sortedObjects_t[objectCount];
  clearArray(m_sourceObjects, 0, objectCount-1);

  m_masterObjects = new sortedObjects_t[m_masterLength];
  clearArray(m_masterObjects, 0, m_masterLength-1);

}

ObjectSmoothing::~ObjectSmoothing()
{
  delete[] m_sourceObjects;
  delete[] m_masterObjects;
}


void ObjectSmoothing::clearArray(sortedObjects_t* clearArray, int startIndex, int endIndex)
{
  for (int objectNum = startIndex; objectNum <= endIndex; objectNum++)
  {
    clearArray[objectNum].objSig = VISION_OBJECT_ERR_SIG;
    clearArray[objectNum].objY = 0;
    clearArray[objectNum].objX = 0;
    clearArray[objectNum].objWidth = 0;
    clearArray[objectNum].objHeight = 0;
    clearArray[objectNum].objCenterX = 0;
    clearArray[objectNum].objCenterY = 0;
    clearArray[objectNum].objXVel = 0;
    clearArray[objectNum].objYVel = 0;

    clearArray[objectNum].matchFound = false;
    clearArray[objectNum].lifeCounter = 0;
  }
}


void ObjectSmoothing::swapObjects(sortedObjects_t* swapArray, int firstIndex, int secondIndex)
{
  sortedObjects_t tempObject = {};
  tempObject.objSig = swapArray[firstIndex].objSig;
  tempObject.objX = swapArray[firstIndex].objX;
  tempObject.objY = swapArray[firstIndex].objY;
  tempObject.objWidth = swapArray[firstIndex].objWidth;
  tempObject.objHeight = swapArray[firstIndex].objHeight;
  tempObject.objCenterX = swapArray[firstIndex].objCenterX;
  tempObject.objCenterY = swapArray[firstIndex].objCenterY;
  tempObject.objXVel = swapArray[firstIndex].objXVel;
  tempObject.objYVel = swapArray[firstIndex].objYVel;
  tempObject.matchFound = swapArray[firstIndex].matchFound;
  tempObject.lifeCounter = swapArray[firstIndex].lifeCounter;

  swapArray[firstIndex].objSig = swapArray[secondIndex].objSig;
  swapArray[firstIndex].objX = swapArray[secondIndex].objX;
  swapArray[firstIndex].objY = swapArray[secondIndex].objY;
  swapArray[firstIndex].objWidth = swapArray[secondIndex].objWidth;
  swapArray[firstIndex].objHeight = swapArray[secondIndex].objHeight;
  swapArray[firstIndex].objCenterX = swapArray[secondIndex].objCenterX;
  swapArray[firstIndex].objCenterY = swapArray[secondIndex].objCenterY;
  swapArray[firstIndex].objXVel = swapArray[secondIndex].objXVel;
  swapArray[firstIndex].objYVel = swapArray[secondIndex].objYVel;
  swapArray[firstIndex].matchFound = swapArray[secondIndex].matchFound;
  swapArray[firstIndex].lifeCounter = swapArray[secondIndex].lifeCounter;

  swapArray[secondIndex].objSig = tempObject.objSig;
  swapArray[secondIndex].objX = tempObject.objX;
  swapArray[secondIndex].objY = tempObject.objY;
  swapArray[secondIndex].objWidth = tempObject.objWidth;
  swapArray[secondIndex].objHeight = tempObject.objHeight;
  swapArray[secondIndex].objCenterX = tempObject.objCenterX;
  swapArray[secondIndex].objCenterY = tempObject.objCenterY;
  swapArray[secondIndex].objXVel = tempObject.objXVel;
  swapArray[secondIndex].objYVel = tempObject.objYVel;
  swapArray[secondIndex].matchFound = tempObject.matchFound;
  swapArray[secondIndex].lifeCounter = tempObject.lifeCounter;
}


void ObjectSmoothing::sortArrayY(sortedObjects_t* sortArray, int firstIndex, int lastIndex)
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
      if (sortArray[currentIndex].objCenterY > sortArray[greatestIndex].objCenterY)
      {
        greatestIndex = currentIndex;
        swapNeeded = true;
      }
    }
    if(swapNeeded) swapObjects(sortArray, startIndex, greatestIndex); //Swap with the greatest
  }
}

void ObjectSmoothing::sortArrayLife(sortedObjects_t* sortArray, int firstIndex, int lastIndex)
{
  // Loop through each object looking to swap the largest object to the right
  // except the last one, which will already be sorted by the time we get there
  int swapCount = 0;
  for (int startIndex = firstIndex; startIndex < lastIndex; startIndex++)
  {
    int greatestIndex = startIndex; //Assume current posision to swap
    bool swapNeeded = false;
    // Loop between current and end looking for greatest object
    if(sortArray[startIndex].lifeCounter < m_lifeMax) //Only swap if nessesary
    {
      for (int currentIndex = startIndex + 1; currentIndex <= lastIndex; currentIndex++)
      {
        //If current object is greater than greatest object
        if (sortArray[currentIndex].lifeCounter > sortArray[greatestIndex].lifeCounter)
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
  if(m_debugInfo) std::cout << " | " << "L Swap: " << swapCount;
}


//Imports the source array and sorts it by Y into sourceObjects
void ObjectSmoothing::importSource(objectContainer_t* importContainer)
{
  //Amount of objects to read. Not to exceed sourceCount
  m_sourceCount = importContainer->objectCount > m_sourceLength ? m_sourceLength : importContainer->objectCount;
  //Copies source into m_source array
  for (int objectNum = 0; objectNum < m_sourceCount; objectNum++)
  {
    if(importContainer->objectArray[objectNum].objSig != VISION_OBJECT_ERR_SIG)
    {
      m_sourceObjects[objectNum].objSig = importContainer->objectArray[objectNum].objSig;
      m_sourceObjects[objectNum].objX = importContainer->objectArray[objectNum].objX;
      m_sourceObjects[objectNum].objY = importContainer->objectArray[objectNum].objY;
      m_sourceObjects[objectNum].objWidth = importContainer->objectArray[objectNum].objWidth;
      m_sourceObjects[objectNum].objHeight = importContainer->objectArray[objectNum].objHeight;
      m_sourceObjects[objectNum].objCenterX = importContainer->objectArray[objectNum].objCenterX;
      m_sourceObjects[objectNum].objCenterY = importContainer->objectArray[objectNum].objCenterY;
      m_sourceObjects[objectNum].matchFound = false;
      m_sourceObjects[objectNum].lifeCounter = 0;
    }
    else std::cout << "Invalid Source: Err" << objectNum << "\n";
  }
  //Resets the remaining slots in the m_sourceObjects
  clearArray(m_sourceObjects, m_sourceCount, m_sourceLength-1);
  sortArrayY(m_sourceObjects, 0, m_sourceCount-1); //Sorts m_sourceObjects by Y
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

  destObject.lifeCounter++; //Increase life
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
  if(destObject.lifeCounter > m_lifeMax) destObject.lifeCounter = m_lifeMax;
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
  for(int sourceObjectNum = 0; sourceObjectNum < m_sourceCount; sourceObjectNum++)
  {
    sourceMatchFound = false;
    //loop through all current master objects
    for(int masterNum = 0; masterNum < m_masterCount && !sourceMatchFound; masterNum++)
    {
      if(!m_masterObjects[masterNum].matchFound)
      {
        if(compareObjects(&m_masterObjects[masterNum], &m_sourceObjects[sourceObjectNum]))
        {
          mergeCount++;
          mergeObject(m_masterObjects[masterNum], m_sourceObjects[sourceObjectNum]);
          m_masterObjects[masterNum].matchFound = true;
          sourceMatchFound = true;
        }
      }
    }
    //If no match was found, if all matches have already been made, or if there are more sources than master
    if(!sourceMatchFound)
    {
      pushCount++;
      //Add extra object to the end of master
      pushObject(m_masterObjects[m_masterCount], m_sourceObjects[sourceObjectNum]);
      m_masterObjects[m_masterCount].matchFound = true;
      newMasterCount++; //Let merge know there are extra objects
    }
  }
  for(int masterNum = 0; masterNum < m_masterCount; masterNum++) //Scan through master looking for non-matches
  {
    if(!m_masterObjects[masterNum].matchFound)
    {
      trimCount++;
      trimObject(m_masterObjects[masterNum]);
    }
  }

  m_masterCount = newMasterCount;
  if(m_debugInfo) std::cout << std::endl << "Count:" << m_masterCount << " | " << "M:" << mergeCount << " " << "P:" << pushCount << " " << "T:" << trimCount;

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
    m_masterObjects[masterNum].matchFound = false;
  }

  int purgeCount = 0;

  int newMasterCount = m_masterCount;
  bool endScan = false;
  for(int masterNum = 0; masterNum < m_masterCount && !endScan; masterNum++) //Clear match bool
  {
    if(m_masterObjects[masterNum].lifeCounter <= 0)
    {
      purgeCount++;
      newMasterCount--;
    }
  }
  if(m_debugInfo) std::cout << " | " << "R:" << purgeCount;
  sortArrayLife(m_masterObjects, 0, m_masterCount); //Moves all 0 objects to the right
  m_masterCount = newMasterCount;

  int startIndex = 0;
  for(int lifeCount = m_lifeMax; lifeCount > m_lifeMax - m_lifeThreshold && startIndex < m_masterCount; lifeCount--)
  {
    int searchBoundary = startIndex;
    bool sortNeeded = false;
    while(m_masterObjects[searchBoundary].lifeCounter >= lifeCount && searchBoundary < m_masterCount)
    {
      sortNeeded = true;
      searchBoundary++;
    }
    if(sortNeeded && startIndex != searchBoundary-1)
    {
      if(m_debugInfo) std::cout << " | Y Sort: " << startIndex << "-" << searchBoundary-1;
      sortArrayY(m_masterObjects, startIndex, searchBoundary-1);
    }
    startIndex = searchBoundary;
  }

  clearArray(m_masterObjects, m_masterCount, m_masterLength);
}


void ObjectSmoothing::exportObjects(objectContainer_t* exportContainer, int minLife, int maxLife)
{
  int exportCount = 0;
  //for each object in master down to x life
  for(int objectNum = 0; objectNum < exportContainer->arrayLength; objectNum++)
  {
    if(m_masterObjects[objectNum].lifeCounter <= maxLife && m_masterObjects[objectNum].lifeCounter >= minLife)
    {
      exportContainer->objectArray[objectNum].objSig = m_masterObjects[objectNum].objSig;
      exportContainer->objectArray[objectNum].objX = m_masterObjects[objectNum].objX;
      exportContainer->objectArray[objectNum].objY = m_masterObjects[objectNum].objY;
      exportContainer->objectArray[objectNum].objWidth = m_masterObjects[objectNum].objWidth;
      exportContainer->objectArray[objectNum].objHeight = m_masterObjects[objectNum].objHeight;
      exportContainer->objectArray[objectNum].objSize = (m_masterObjects[objectNum].objWidth + m_masterObjects[objectNum].objHeight) / 2;
      exportContainer->objectArray[objectNum].objCenterX = m_masterObjects[objectNum].objCenterX;
      exportContainer->objectArray[objectNum].objCenterY = m_masterObjects[objectNum].objCenterY;
      exportContainer->objectArray[objectNum].discardObject = false;
      exportCount = objectNum + 1;
    }
    else
    {
      exportContainer->objectArray[objectNum].objSig = VISION_OBJECT_ERR_SIG;
      exportContainer->objectArray[objectNum].objX = 0;
      exportContainer->objectArray[objectNum].objY = 0;
      exportContainer->objectArray[objectNum].objWidth = 0;
      exportContainer->objectArray[objectNum].objHeight = 0;
      exportContainer->objectArray[objectNum].objSize = 0;
      exportContainer->objectArray[objectNum].objCenterX = 0;
      exportContainer->objectArray[objectNum].objCenterY = 0;
      exportContainer->objectArray[objectNum].discardObject = false;
    }
  }
  exportContainer->objectCount = exportCount;
}



void ObjectSmoothing::smoothObjects(objectContainer_t* inContainer, objectContainer_t* outContainer)
{
  importSource(inContainer);
  mergeObjects();
  sortMaster();

  exportObjects(outContainer, m_lifeMax - m_lifeThreshold, m_lifeMax);
}
