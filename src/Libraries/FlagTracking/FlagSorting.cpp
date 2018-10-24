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





//Imports the source array and sorts it by Y into sourceArray
void FlagSorting::importSource(simpleObjects_t* sourceObjects, int currentSourceCount)
{
  //Amount of objects to read. Not to exceed sourceCount
  m_currentSourceCount = currentSourceCount > m_sourceCount ? m_sourceCount : currentSourceCount;


  for (int objectNum = 0; objectNum < currentSourceCount; objectNum++) //Copies source
  {
    if(sourceObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG)
    {
      m_sourceObjects[objectNum].objSig = sourceObjects[objectNum].objSig;
      m_sourceObjects[objectNum].objX = sourceObjects[objectNum].objX;
      m_sourceObjects[objectNum].objY = sourceObjects[objectNum].objY;
      m_sourceObjects[objectNum].objWidth = sourceObjects[objectNum].objWidth;
      m_sourceObjects[objectNum].objHeight = sourceObjects[objectNum].objHeight;
      m_sourceObjects[objectNum].objCenterX = sourceObjects[objectNum].objCenterX;
      m_sourceObjects[objectNum].objCenterY = sourceObjects[objectNum].objCenterY;
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



  //now sort m_sourceObjects



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
