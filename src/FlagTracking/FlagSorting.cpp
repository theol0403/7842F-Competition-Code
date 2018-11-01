#include "main.h"

#include "Include/FlagTracking/FlagTracking.hpp"
#include "Include/FlagTracking/FlagSorting.hpp"


FlagSorting::FlagSorting(int objectCount, int maxLife, float emaAlpha, int objectPosThreshold)
:
m_sourceLength{objectCount},
m_masterLength{objectCount * maxLife},
m_maxLife{maxLife},
m_emaAlpha{emaAlpha},
m_objectPosThreshold{}
{
  m_sourceObjects = new sortedObjects_t[objectCount];
  clearArray(m_sourceObjects, 0, objectCount-1);

  m_tempAllignIndex = new int[m_masterLength];
  m_masterObjects = new sortedObjects_t[m_masterLength];
  clearArray(m_masterObjects, 0, m_masterLength-1);

  m_exportObjects = new simpleObjects_t[objectCount];
}

FlagSorting::~FlagSorting()
{
  delete[] m_sourceObjects;
  delete[] m_tempAllignIndex;
  delete[] m_masterObjects;
  delete[] m_exportObjects;
}


void FlagSorting::clearArray(sortedObjects_t* clearArray, int startIndex, int endIndex)
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
    clearArray[objectNum].matchFound = false;
    clearArray[objectNum].lifeCounter = 0;
  }
}


void FlagSorting::swapObjects(sortedObjects_t* swapArray, int firstIndex, int secondIndex)
{
  sortedObjects_t tempObject;

  tempObject.objSig = swapArray[firstIndex].objSig;
  tempObject.objX = swapArray[firstIndex].objX;
  tempObject.objY = swapArray[firstIndex].objY;
  tempObject.objWidth = swapArray[firstIndex].objWidth;
  tempObject.objHeight = swapArray[firstIndex].objHeight;
  tempObject.objCenterX = swapArray[firstIndex].objCenterX;
  tempObject.objCenterY = swapArray[firstIndex].objCenterY;
  tempObject.matchFound = swapArray[firstIndex].matchFound;
  tempObject.lifeCounter = swapArray[firstIndex].lifeCounter;

  swapArray[firstIndex].objSig = swapArray[secondIndex].objSig;
  swapArray[firstIndex].objX = swapArray[secondIndex].objX;
  swapArray[firstIndex].objY = swapArray[secondIndex].objY;
  swapArray[firstIndex].objWidth = swapArray[secondIndex].objWidth;
  swapArray[firstIndex].objHeight = swapArray[secondIndex].objHeight;
  swapArray[firstIndex].objCenterX = swapArray[secondIndex].objCenterX;
  swapArray[firstIndex].objCenterY = swapArray[secondIndex].objCenterY;
  swapArray[firstIndex].matchFound = swapArray[secondIndex].matchFound;
  swapArray[firstIndex].lifeCounter = swapArray[secondIndex].lifeCounter;

  swapArray[secondIndex].objSig = tempObject.objSig;
  swapArray[secondIndex].objX = tempObject.objX;
  swapArray[secondIndex].objY = tempObject.objY;
  swapArray[secondIndex].objWidth = tempObject.objWidth;
  swapArray[secondIndex].objHeight = tempObject.objHeight;
  swapArray[secondIndex].objCenterX = tempObject.objCenterX;
  swapArray[secondIndex].objCenterY = tempObject.objCenterY;
  swapArray[secondIndex].matchFound = tempObject.matchFound;
  swapArray[secondIndex].lifeCounter = tempObject.lifeCounter;
}


void FlagSorting::sortArrayY(sortedObjects_t* sortArray, int firstIndex, int lastIndex)
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
      if (sortArray[currentIndex].objY > sortArray[greatestIndex].objY)
      {
        greatestIndex = currentIndex;
        swapNeeded = true;
      }
    }
    if(swapNeeded) swapObjects(sortArray, startIndex, greatestIndex); //Swap with the greatest
  }
}



//Imports the source array and sorts it by Y into sourceObjects
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
  }
  sortArrayY(m_sourceObjects, 0, m_sourceCount-1); //Sorts m_sourceObjects by Y
}





bool FlagSorting::compareObjects(sortedObjects_t &sourceObject, sortedObjects_t &masterObject)
{
  //compare sig, then Y, then X, then size
  bool cascadingTrue = true;
  cascadingTrue = (cascadingTrue && (sourceObject.objSig == masterObject.objSig));
  cascadingTrue = (cascadingTrue && ((sourceObject.objCenterY > masterObject.objCenterY - m_objectPosThreshold) && (sourceObject.objCenterY < masterObject.objCenterY + m_objectPosThreshold)));
  cascadingTrue = (cascadingTrue && ((sourceObject.objCenterX > masterObject.objCenterX - m_objectPosThreshold) && (sourceObject.objCenterX < masterObject.objCenterX + m_objectPosThreshold)));
  //cascadingTrue = cascadingTrue && sourceObject.objWidth > masterObject.objWidth - 10 && sourceObject.objWidth < masterObject.objWidth + 10;
  //  cascadingTrue = cascadingTrue && sourceObject.objY > masterObject.objY - 10 && sourceObject.objY < masterObject.objY + 10;

  return cascadingTrue;
}




void FlagSorting::createAllignList()
{

  //take object 0
  //Search masterArray for same X, than Y
  //put objectNum in temp array with same index as master
  //mark master as a match found
  //mark object as match found
  //if no match put it in m_masterCount + 1 and increment it
  //repeate for next object

  //clear temp array
  for(int indexNum = 0; indexNum < m_masterLength; indexNum++)
  {
    m_tempAllignIndex[indexNum] = -1; //-1 Represents no match, as 0 would be a match with object 0
  }

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
    //If no match was found, all matches have already been made, or if there are more sources than master
    if(!m_sourceObjects[sourceObjectNum].matchFound)
    {
      //Add extra object to the end of master
      m_tempAllignIndex[m_tempCount] = sourceObjectNum;
      m_tempCount++; //Let merge know there are extra objects
    }

  }


  //allign sorted source array into temp array symetrical to master, left to right
  //any non-found objects to into arrays past masterCount
}


void FlagSorting::debugAllign()
{
  std::cout << "Master Count | " << m_masterCount;
  std::cout << " | Temp Count | " << m_tempCount;
  std::cout << " | Temp Allign";
  for(int tempNum = 0; tempNum < 5; tempNum++)
  {
    std::cout << " | " << m_tempAllignIndex[tempNum];
  }
  std::cout << "\n";
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
    if(m_tempAllignIndex[masterNum] != -1) //If a match was found with master
    {
      //merge objects
      m_masterObjects[masterNum].objSig = m_sourceObjects[m_tempAllignIndex[masterNum]].objSig;
      m_masterObjects[masterNum].objY = emaCalculate(m_masterObjects[masterNum].objY, m_sourceObjects[m_tempAllignIndex[masterNum]].objY);
      m_masterObjects[masterNum].objX = emaCalculate(m_masterObjects[masterNum].objX, m_sourceObjects[m_tempAllignIndex[masterNum]].objX);
      m_masterObjects[masterNum].objWidth = emaCalculate(m_masterObjects[masterNum].objWidth, m_sourceObjects[m_tempAllignIndex[masterNum]].objWidth);
      m_masterObjects[masterNum].objHeight = emaCalculate(m_masterObjects[masterNum].objHeight, m_sourceObjects[m_tempAllignIndex[masterNum]].objHeight);
      m_masterObjects[masterNum].objCenterX = emaCalculate(m_masterObjects[masterNum].objCenterX, m_sourceObjects[m_tempAllignIndex[masterNum]].objCenterX);
      m_masterObjects[masterNum].objCenterY = emaCalculate(m_masterObjects[masterNum].objCenterY, m_sourceObjects[m_tempAllignIndex[masterNum]].objCenterY);
      m_masterObjects[masterNum].matchFound = false;
      m_masterObjects[masterNum].lifeCounter++; //Increase life
      if(m_masterObjects[masterNum].lifeCounter > m_maxLife) m_masterObjects[masterNum].lifeCounter = m_maxLife;
    }
    else //If no match for an object in master
    {
      m_masterObjects[masterNum].lifeCounter--;
    }
  }

  //Loop through all extra new objects in temp and fill into master
  for(int masterNum = m_masterCount; masterNum < m_tempCount; masterNum++)
  {
    m_masterObjects[masterNum].objSig = m_sourceObjects[m_tempAllignIndex[masterNum]].objSig;
    m_masterObjects[masterNum].objY = m_sourceObjects[m_tempAllignIndex[masterNum]].objY;
    m_masterObjects[masterNum].objX = m_sourceObjects[m_tempAllignIndex[masterNum]].objX;
    m_masterObjects[masterNum].objWidth = m_sourceObjects[m_tempAllignIndex[masterNum]].objWidth;
    m_masterObjects[masterNum].objHeight = m_sourceObjects[m_tempAllignIndex[masterNum]].objHeight;
    m_masterObjects[masterNum].objCenterX = m_sourceObjects[m_tempAllignIndex[masterNum]].objCenterX;
    m_masterObjects[masterNum].objCenterY = m_sourceObjects[m_tempAllignIndex[masterNum]].objCenterY;
    m_masterObjects[masterNum].matchFound = false;
    m_masterObjects[masterNum].lifeCounter = 1;
  }

  m_masterCount = m_tempCount; //Master now contains the new objects

}







void FlagSorting::sortMaster()
{

  int firstIndex = 0;

  for(int lifeCounter = m_maxLife; m_maxLife > 0; lifeCounter--) //Start at top life, end at 1
  {

    bool abortScan = false; //Stop scanning for life when no more objects with proper life
    int lastMatch = 0;

    //For each master element except from the last
    for (int startIndex = firstIndex; startIndex < m_masterLength - 1 && !abortScan; startIndex++)
    {
      //If startIndex needs swapping
      if(m_masterObjects[startIndex].lifeCounter != lifeCounter)
      {
        bool swapFound = false; //Exit if a swap has been made
        // Loop between startIndex and end of master looking for proper life
        for(int currentIndex = startIndex + 1; currentIndex < m_masterLength && !swapFound; currentIndex++)
        {
          if(m_masterObjects[currentIndex].lifeCounter == lifeCounter) //If match is found to the right
          {
            swapObjects(m_masterObjects, startIndex, currentIndex); //Swap
            swapFound = true; //Exit looping and move on to next startIndex
            lastMatch = startIndex; //Mark index as having proper life
          }
          //else keep on looping through objects
        }
        if(!swapFound) //If no proper life was found
        {
          //Stop looping and move on to next life
          abortScan = true;
        }
      }
      //If startIndex already has proper life
      else
      {
        //Mark this index as having proper life and move on to next index
        lastMatch = startIndex;
      }
    }
    //now lastMatch contains the last index that has the proper life

    sortArrayY(m_masterObjects, firstIndex, lastMatch);

    firstIndex = lastMatch + 1; //First index is now the start index of the next object group
  }

  //Delete objects to the right of life 1, which is 0 life
  clearArray(m_masterObjects, firstIndex, m_masterLength-1);

  m_masterCount = firstIndex; //Master count contains the number of good objects in the master

  //sort by life
  //sort by Y
  //updates masterCount
}





simpleObjects_t* FlagSorting::exportObjects()
{
  m_exportCount = 0;
  //for each object in master down to 5 life
  for(int objectNum = 0; objectNum < m_sourceLength; objectNum++)
  {
    if(m_masterObjects[objectNum].lifeCounter >= m_maxLife - 1)
    {
      m_exportObjects[objectNum].objSig = m_masterObjects[objectNum].objSig;
      m_exportObjects[objectNum].objX = m_masterObjects[objectNum].objX;
      m_exportObjects[objectNum].objY = m_masterObjects[objectNum].objY;
      m_exportObjects[objectNum].objWidth = m_masterObjects[objectNum].objWidth;
      m_exportObjects[objectNum].objHeight = m_masterObjects[objectNum].objHeight;
      m_exportObjects[objectNum].objSize = (m_masterObjects[objectNum].objWidth + m_masterObjects[objectNum].objHeight) / 2;
      m_exportObjects[objectNum].objCenterX = m_masterObjects[objectNum].objCenterX;
      m_exportObjects[objectNum].objCenterY = m_masterObjects[objectNum].objCenterY;
      m_exportObjects[objectNum].discardObject = false;
      m_exportCount = objectNum + 1;
    }
    else
    {
      m_exportObjects[objectNum].objSig = VISION_OBJECT_ERR_SIG;
      m_exportObjects[objectNum].objX = 0;
      m_exportObjects[objectNum].objY = 0;
      m_exportObjects[objectNum].objWidth = 0;
      m_exportObjects[objectNum].objHeight = 0;
      m_exportObjects[objectNum].objSize = 0;
      m_exportObjects[objectNum].objCenterX = 0;
      m_exportObjects[objectNum].objCenterY = 0;
      m_exportObjects[objectNum].discardObject = false;
    }
  }
  return m_exportObjects;
}

int FlagSorting::exportCount()
{
  return m_exportCount;
}
