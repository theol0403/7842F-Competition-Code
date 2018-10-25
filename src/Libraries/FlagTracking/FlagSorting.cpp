#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTracking.hpp"
#include "Include/Libraries/FlagTracking/FlagSorting.hpp"


FlagSorting::FlagSorting(int sourceCount, int maxLife)
: m_sourceCount{sourceCount}, m_maxLife{maxLife}, m_masterCount{sourceCount * maxLife}
{
  m_sourceObjects = new sortedObjects_t[sourceCount];
  m_masterObjects = new sortedObjects_t[m_masterCount];
}

FlagSorting::~FlagSorting()
{
  delete[] m_sourceObjects;
  delete[] m_masterObjects;
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
  tempObject.lifeCounter = swapArray[firstObject].lifeCounter;
  tempObject.matchFound = swapArray[firstObject].matchFound;

  swapArray[firstObject].objSig = swapArray[secondObject].objSig;
  swapArray[firstObject].objX = swapArray[secondObject].objX;
  swapArray[firstObject].objY = swapArray[secondObject].objY;
  swapArray[firstObject].objWidth = swapArray[secondObject].objWidth;
  swapArray[firstObject].objHeight = swapArray[secondObject].objHeight;
  swapArray[firstObject].objCenterX = swapArray[secondObject].objCenterX;
  swapArray[firstObject].objCenterY = swapArray[secondObject].objCenterY;
  swapArray[firstObject].lifeCounter = swapArray[secondObject].lifeCounter;
  swapArray[firstObject].matchFound = swapArray[secondObject].matchFound;

  swapArray[secondObject].objSig = tempObject.objSig;
  swapArray[secondObject].objX = tempObject.objX;
  swapArray[secondObject].objY = tempObject.objY;
  swapArray[secondObject].objWidth = tempObject.objWidth;
  swapArray[secondObject].objHeight = tempObject.objHeight;
  swapArray[secondObject].objCenterX = tempObject.objCenterX;
  swapArray[secondObject].objCenterY = tempObject.objCenterY;
  swapArray[secondObject].lifeCounter = tempObject.lifeCounter;
  swapArray[secondObject].matchFound = tempObject.matchFound;
}


void FlagSorting::sortArrayY(sortedObjects_t* sortArray, int arrayLength)
{
  // Loop through each object looking to swap the largest object to the right
  // except the last one, which will already be sorted by the time we get there
  for (int startIndex = 0; startIndex < arrayLength - 1; startIndex++)
  {
    int largestIndex = startIndex; //Assume current posision to swap
    bool swapNeeded = false;

    // Loop between current and end looking for larger object
    for (int currentIndex = startIndex + 1; currentIndex < arrayLength; currentIndex++)
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
  m_currentSourceCount = currentSourceCount > m_sourceCount ? m_sourceCount : currentSourceCount;
  

  for (int objectNum = 0; objectNum < currentSourceCount; objectNum++) //Copies source into m_source array
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
      m_sourceObjects[objectNum].lifeCounter = 0;
      m_sourceObjects[objectNum].matchFound = 0;
    }
    else
    {
      std::cout << "SORTING: ERR_SIG IN SOURCE " << objectNum << "\n";
    }
  }


  for (int objectNum = currentSourceCount; objectNum < m_sourceCount; objectNum++) //cleans the rest
  {
    m_sourceObjects[objectNum].objSig = VISION_OBJECT_ERR_SIG;
    m_sourceObjects[objectNum].objY = 0;
    m_sourceObjects[objectNum].objX = 0;
    m_sourceObjects[objectNum].objWidth = 0;
    m_sourceObjects[objectNum].objHeight = 0;
    m_sourceObjects[objectNum].objCenterX = 0;
    m_sourceObjects[objectNum].objCenterY = 0;
    m_sourceObjects[objectNum].lifeCounter = 0;
    m_sourceObjects[objectNum].matchFound = 0;
  }

  sortArrayY(m_sourceObjects, currentSourceCount);

}






void allignObjects()
{
  //clear temp array
  //allign sorted source array into temp array symetrical to master, left to right
  //any non-found objects to into arrays past masterCount
}

void mergeObjects(); //merges arrays using EMA //updates the life of the master elements


void FlagSorting::sortMaster()
{
  //Sorts master array by Y, and danging sorted by life than y at the end of the array
  //updates masterCount
}
