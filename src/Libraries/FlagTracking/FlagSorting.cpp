#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTracking.hpp"
#include "Include/Libraries/FlagTracking/FlagSorting.hpp"


FlagSorting::FlagSorting(int objectCount, int maxLife, float emaAlpha = 1)
:
m_sourceLength{objectCount},
m_masterLength{objectCount * maxLife},
m_maxLife{maxLife},
m_emaAlpha{emaAlpha}
{
  m_sourceObjects = new sortedObjects_t[objectCount];
  m_tempAllignIndex = new int[m_masterLength];
  m_masterObjects = new sortedObjects_t[m_masterLength];
}

FlagSorting::~FlagSorting()
{
  delete[] m_sourceObjects;
  delete[] m_tempAllignIndex;
  delete[] m_masterObjects;
}


void FlagSorting::clearArray(sortedObjects_t* clearArray, int startObject, int endObject)
{
  for (int objectNum = startObject; objectNum < endObject; objectNum++)
  {
    clearArray[objectNum].objSig = VISION_OBJECT_ERR_SIG;
    clearArray[objectNum].objY = 0;
    clearArray[objectNum].objX = 0;
    clearArray[objectNum].objWidth = 0;
    clearArray[objectNum].objHeight = 0;
    clearArray[objectNum].objCenterX = 0;
    clearArray[objectNum].objCenterY = 0;
    clearArray[objectNum].matchFound = false;
    clearArray[objectNum].lifeCounter = 0;
    clearArray[objectNum].visibility = false;
  }
}


void FlagSorting::swapObjects(sortedObjects_t* swapArray, int firstObject, int secondObject)
{
  sortedObjects_t tempObject;

  tempObject.objSig = swapArray[firstObject].objSig;
  tempObject.objX = swapArray[firstObject].objX;
  tempObject.objY = swapArray[firstObject].objY;
  tempObject.objWidth = swapArray[firstObject].objWidth;
  tempObject.objHeight = swapArray[firstObject].objHeight;
  tempObject.objCenterX = swapArray[firstObject].objCenterX;
  tempObject.objCenterY = swapArray[firstObject].objCenterY;
  tempObject.matchFound = swapArray[firstObject].matchFound;
  tempObject.lifeCounter = swapArray[firstObject].lifeCounter;
  tempObject.visibility = swapArray[firstObject].visibility;

  swapArray[firstObject].objSig = swapArray[secondObject].objSig;
  swapArray[firstObject].objX = swapArray[secondObject].objX;
  swapArray[firstObject].objY = swapArray[secondObject].objY;
  swapArray[firstObject].objWidth = swapArray[secondObject].objWidth;
  swapArray[firstObject].objHeight = swapArray[secondObject].objHeight;
  swapArray[firstObject].objCenterX = swapArray[secondObject].objCenterX;
  swapArray[firstObject].objCenterY = swapArray[secondObject].objCenterY;
  swapArray[firstObject].matchFound = swapArray[secondObject].matchFound;
  swapArray[firstObject].lifeCounter = swapArray[secondObject].lifeCounter;
  swapArray[firstObject].visibility = swapArray[secondObject].visibility;

  swapArray[secondObject].objSig = tempObject.objSig;
  swapArray[secondObject].objX = tempObject.objX;
  swapArray[secondObject].objY = tempObject.objY;
  swapArray[secondObject].objWidth = tempObject.objWidth;
  swapArray[secondObject].objHeight = tempObject.objHeight;
  swapArray[secondObject].objCenterX = tempObject.objCenterX;
  swapArray[secondObject].objCenterY = tempObject.objCenterY;
  swapArray[secondObject].matchFound = tempObject.matchFound;
  swapArray[secondObject].lifeCounter = tempObject.lifeCounter;
  swapArray[secondObject].visibility = tempObject.visibility;
}


void FlagSorting::sortArrayY(sortedObjects_t* sortArray, int startIndex, int lastIndex)
{
  // Loop through each object looking to swap the largest object to the right
  // except the last one, which will already be sorted by the time we get there
  for (int startIndex = startIndex; startIndex < lastIndex - 1; startIndex++)
  {
    int largestIndex = startIndex; //Assume current posision to swap
    bool swapNeeded = false;
    // Loop between current and end looking for larger object
    for (int currentIndex = startIndex + 1; currentIndex < lastIndex; currentIndex++)
    {
      if (sortArray[currentIndex].objY > sortArray[largestIndex].objY)
      {
        largestIndex = currentIndex;
        swapNeeded = true;
      }
    }
    if(swapNeeded) swapObjects(sortArray, startIndex, largestIndex);
  }
}



//Imports the source array and sorts it by Y into sourceArray
void FlagSorting::importSource(simpleObjects_t* importObjects, int currentSourceCount)
{
  //Amount of objects to read. Not to exceed sourceCount
  m_sourceCount = currentSourceCount > m_sourceLength ? m_sourceLength : currentSourceCount;
  //Copies source into m_source array
  for (int objectNum = 0; objectNum < m_sourceCount; objectNum++)
  {
    if(importObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG)
    {
      m_sourceObjects[objectNum].objSig = importObjects[objectNum].objSig;
      m_sourceObjects[objectNum].objX = importObjects[objectNum].objX;
      m_sourceObjects[objectNum].objY = importObjects[objectNum].objY;
      m_sourceObjects[objectNum].objWidth = importObjects[objectNum].objWidth;
      m_sourceObjects[objectNum].objHeight = importObjects[objectNum].objHeight;
      m_sourceObjects[objectNum].objCenterX = importObjects[objectNum].objCenterX;
      m_sourceObjects[objectNum].objCenterY = importObjects[objectNum].objCenterY;
      m_sourceObjects[objectNum].matchFound = false;
      m_sourceObjects[objectNum].lifeCounter = 0;
      m_sourceObjects[objectNum].visibility = false;
    }
    else
    {
      std::cout << "SORTING: ERR_SIG IN SOURCE " << objectNum << "\n";
    }
  }
  //Resets the remaining slots in the m_sourceObjects
  for (int objectNum = m_sourceCount; objectNum < m_sourceLength; objectNum++)
  {
    m_sourceObjects[objectNum].objSig = VISION_OBJECT_ERR_SIG;
    m_sourceObjects[objectNum].objY = 0;
    m_sourceObjects[objectNum].objX = 0;
    m_sourceObjects[objectNum].objWidth = 0;
    m_sourceObjects[objectNum].objHeight = 0;
    m_sourceObjects[objectNum].objCenterX = 0;
    m_sourceObjects[objectNum].objCenterY = 0;
    m_sourceObjects[objectNum].matchFound = false;
    m_sourceObjects[objectNum].lifeCounter = 0;
    m_sourceObjects[objectNum].visibility = false;
  }
  sortArrayY(m_sourceObjects, 0, m_sourceCount); //Sorts m_sourceObjects by Y
}





bool FlagSorting::compareObjects(sortedObjects_t &sourceObject, sortedObjects_t &masterObject)
{
  //compare sig, then Y, then X, then size
  bool cascadingTrue = true;
  cascadingTrue = cascadingTrue && sourceObject.objSig == masterObject.objSig;
  cascadingTrue = cascadingTrue && sourceObject.objCenterY > masterObject.objCenterY - 10 && sourceObject.objCenterY < masterObject.objCenterY + 10;
  cascadingTrue = cascadingTrue && sourceObject.objCenterX > masterObject.objCenterX - 10 && sourceObject.objCenterX < masterObject.objCenterX + 10;
  //cascadingTrue = cascadingTrue && sourceObject.objWidth > masterObject.objWidth - 10 && sourceObject.objWidth < masterObject.objWidth + 10;
  //  cascadingTrue = cascadingTrue && sourceObject.objY > masterObject.objY - 10 && sourceObject.objY < masterObject.objY + 10;

  return cascadingTrue;
}




void FlagSorting::createAllignList()
{
  //clear temp array
  for(int indexNum = 0; indexNum < m_masterLength; indexNum++)
  {
    m_tempAllignIndex[indexNum] = -1;
  }

  //take object 0
  //Search masterArray for same X, than Y, then life
  //put objectNum in temp array with same index as master
  //mark master as a match found
  //mark object as match found
  //if no match put it in m_masterCount + 1 and increment it
  //repeate for next object

  m_tempCount = m_masterCount; //size of temp is size of master

  //for every source object
  for(int sourceObjectNum = 0; sourceObjectNum < m_sourceCount; sourceObjectNum++)
  {
    //loop through all current master objects
    for(int masterObjectNum = 0; masterObjectNum < m_masterCount && !m_sourceObjects[sourceObjectNum].matchFound; masterObjectNum++)
    {
      if(!m_masterObjects[masterObjectNum].matchFound)
      {
        if(compareObjects(m_sourceObjects[sourceObjectNum], m_masterObjects[masterObjectNum]))
        {
          m_tempAllignIndex[masterObjectNum] = sourceObjectNum;
          m_masterObjects[masterObjectNum].matchFound = true;
          m_sourceObjects[sourceObjectNum].matchFound = true;
        }
      }
    }
    if(!m_sourceObjects[sourceObjectNum].matchFound) //If no match was found
    {
      m_tempAllignIndex[m_tempCount] = sourceObjectNum;
      m_tempCount++;
    }

  }


  //allign sorted source array into temp array symetrical to master, left to right
  //any non-found objects to into arrays past masterCount
}


int FlagSorting::emaCalculate(int lastValue, int newValue)
{
  return (m_emaAlpha * newValue + (1.0 - m_emaAlpha) * lastValue);
}

//merges arrays using EMA //updates the life of the master elements
void FlagSorting::mergeMaster()
{
  for(int masterNum = 0; masterNum < m_masterCount; masterNum++) //loop through existing objects in master
  {
    if(m_tempAllignIndex[masterNum] != -1)
    {
      m_masterObjects[masterNum].objSig = m_sourceObjects[m_tempAllignIndex[masterNum]].objSig;
      m_masterObjects[masterNum].objY = emaCalculate(m_masterObjects[masterNum].objY, m_sourceObjects[m_tempAllignIndex[masterNum]].objY);
      m_masterObjects[masterNum].objX = emaCalculate(m_masterObjects[masterNum].objX, m_sourceObjects[m_tempAllignIndex[masterNum]].objX);
      m_masterObjects[masterNum].objWidth = emaCalculate(m_masterObjects[masterNum].objWidth, m_sourceObjects[m_tempAllignIndex[masterNum]].objWidth);
      m_masterObjects[masterNum].objHeight = emaCalculate(m_masterObjects[masterNum].objHeight, m_sourceObjects[m_tempAllignIndex[masterNum]].objHeight);
      m_masterObjects[masterNum].objCenterX = emaCalculate(m_masterObjects[masterNum].objCenterX, m_sourceObjects[m_tempAllignIndex[masterNum]].objCenterX);
      m_masterObjects[masterNum].objCenterY = emaCalculate(m_masterObjects[masterNum].objCenterY, m_sourceObjects[m_tempAllignIndex[masterNum]].objCenterY);
      m_masterObjects[masterNum].matchFound = false;
      m_masterObjects[masterNum].lifeCounter++;
      if(m_masterObjects[masterNum].lifeCounter > m_maxLife) m_masterObjects[masterNum].lifeCounter = m_maxLife;
      m_masterObjects[masterNum].visibility = true;
    }
    else
    {
      m_masterObjects[masterNum].lifeCounter--;
      m_masterObjects[masterNum].visibility = false;
    }
  }

  for(int masterNum = m_masterCount; masterNum < m_tempCount; masterNum++) //loop through new objects in source
  {
    m_masterObjects[masterNum].objSig = m_sourceObjects[m_tempAllignIndex[masterNum]].objSig;
    m_masterObjects[masterNum].objY = m_masterObjects[masterNum].objY;
    m_masterObjects[masterNum].objX = m_masterObjects[masterNum].objX;
    m_masterObjects[masterNum].objWidth = m_masterObjects[masterNum].objWidth;
    m_masterObjects[masterNum].objHeight = m_masterObjects[masterNum].objHeight;
    m_masterObjects[masterNum].objCenterX = m_masterObjects[masterNum].objCenterX;
    m_masterObjects[masterNum].objCenterY = m_masterObjects[masterNum].objCenterY;
    m_masterObjects[masterNum].matchFound = false;
    m_masterObjects[masterNum].lifeCounter = 1;
    m_masterObjects[masterNum].visibility = true;
  }

  m_masterCount = m_tempCount;

}






int FlagSorting::sortArrayLife(sortedObjects_t* sortArray, int lifeSearch, int startIndex, int lastIndex)
{
  int lastPos = 0;

  for (startIndex = startIndex; startIndex < lastIndex - 1; startIndex++)
  {
    bool swapFound = false;
    if(sortArray[startIndex].lifeCounter != lifeSearch)
    {
      // Loop between current and end looking for larger object
      for(int currentIndex = startIndex + 1; currentIndex < lastIndex && !swapFound; currentIndex++)
      {
        if(sortArray[currentIndex].lifeCounter == lifeSearch)
        {
          swapObjects(sortArray, startIndex, currentIndex);
          swapFound = true;
          lastPos = lastPos;
        }
      }
    }
    else
    {
      lastPos = startIndex;
    }
  }
  return lastPos;
}




void FlagSorting::sortMaster()
{
  int startPosition = 0;
  int endPosision = 0;
  for(int lifeCounter = m_maxLife; m_maxLife > 0; lifeCounter--)
  {
    endPosision = sortArrayLife(m_masterObjects, lifeCounter, startPosition, m_masterLength);
    sortArrayY(m_masterObjects, startPosition, endPosision);
    startPosition = endPosision;
  }

  clearArray(m_masterObjects, startPosition, m_masterLength);



  //sort by life
  //sort by Y

  //Sorts master array by Y, and danging sorted by life than y at the end of the array
  //updates masterCount
}

sortedObjects_t* FlagSorting::exportObjects()
{
  return m_masterObjects;
}
