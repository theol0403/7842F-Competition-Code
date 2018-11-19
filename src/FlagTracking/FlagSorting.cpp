#include "FlagSorting.hpp"


FlagSorting::FlagSorting(int objectCount, int maxLife, float emaAlpha, int objectPosThreshold)
:
m_sourceLength{objectCount},
m_masterLength{objectCount * maxLife},
m_maxLife(maxLife),
m_emaAlpha{emaAlpha},
m_objectPosThreshold{objectPosThreshold}
{
  m_sourceObjects = new sortedObjects_t[objectCount];
  clearArray(m_sourceObjects, 0, objectCount-1);

  m_masterObjects = new sortedObjects_t[m_masterLength];
  clearArray(m_masterObjects, 0, m_masterLength-1);

  m_exportObjects = new simpleObjects_t[objectCount];
}

FlagSorting::~FlagSorting()
{
  delete[] m_sourceObjects;
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
  std::cout << "CURRENT COUNT" << currentSourceCount << "\n";
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
    else std::cout << "SORTING: ERR_SIG IN SOURCE " << objectNum << "\n";
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
  // cascadingTrue = cascadingTrue && sourceObject.objY > masterObject.objY - 10 && sourceObject.objY < masterObject.objY + 10;
  if(cascadingTrue) {std::cout << "Match Found \n";};
  return cascadingTrue;
}


int FlagSorting::emaCalculate(int lastValue, int newValue, float emaAlpha)
{
  return (emaAlpha * newValue + (1.0 - emaAlpha) * lastValue);
}

void FlagSorting::mergeObject(sortedObjects_t &destObject, sortedObjects_t &newObject)
{
  destObject.objSig = newObject.objSig;
  destObject.objY = emaCalculate(destObject.objY, newObject.objY, 0.5);
  destObject.objX = emaCalculate(destObject.objX, newObject.objX, 0.5);
  destObject.objWidth = emaCalculate(destObject.objWidth, newObject.objWidth, 0.1);
  destObject.objHeight = emaCalculate(destObject.objHeight, newObject.objHeight, 0.1);
  destObject.objCenterX = emaCalculate(destObject.objCenterX, newObject.objCenterX, 0.5);
  destObject.objCenterY = emaCalculate(destObject.objCenterY, newObject.objCenterY, 0.5);
  destObject.matchFound = true;
  destObject.lifeCounter++; //Increase life
  if(destObject.lifeCounter > m_maxLife) destObject.lifeCounter = m_maxLife;
}

void FlagSorting::pushObject(sortedObjects_t &destObject, sortedObjects_t &newObject)
{
  destObject.objSig = newObject.objSig;
  destObject.objY = newObject.objY;
  destObject.objX = newObject.objX;
  destObject.objWidth = newObject.objWidth;
  destObject.objHeight = newObject.objHeight;
  destObject.objCenterX = newObject.objCenterX;
  destObject.objCenterY = newObject.objCenterY;
  destObject.matchFound = true;
  destObject.lifeCounter = 1;
  if(destObject.lifeCounter > m_maxLife) destObject.lifeCounter = m_maxLife;
}

void FlagSorting::trimObject(sortedObjects_t &destObject)
{
  destObject.lifeCounter--;
}


void FlagSorting::mergeObjects()
{
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
        if(compareObjects(m_masterObjects[masterNum], m_sourceObjects[sourceObjectNum]))
        {
          mergeObject(m_masterObjects[masterNum], m_sourceObjects[sourceObjectNum]);
          sourceMatchFound = true;
        }
      }
    }
    //If no match was found, if all matches have already been made, or if there are more sources than master
    if(!sourceMatchFound)
    {
      //Add extra object to the end of master
      pushObject(m_masterObjects[m_masterCount], m_sourceObjects[sourceObjectNum]);
      m_masterCount++; //Let merge know there are extra objects
    }
  }

  for(int masterNum = 0; masterNum < m_masterCount; masterNum++) //Scan through master looking for non-matches
  {
    if(!m_masterObjects[masterNum].matchFound)
    {
      trimObject(m_masterObjects[masterNum]);
    }
  }

}


void FlagSorting::sortMaster()
{

  for(int masterNum = 0; masterNum > m_masterLength; masterNum++) //Clear match bool
  {
    m_masterObjects[masterNum].matchFound = false;
  }

  int firstIndex = 0; //Index of master to start looking through

  for(int lifeCounter = m_maxLife; lifeCounter > 0; lifeCounter--) //Start at top life, end at 1
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
  clearArray(m_masterObjects, firstIndex, m_masterLength - 1);

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
