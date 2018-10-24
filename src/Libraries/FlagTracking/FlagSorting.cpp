#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTracking.hpp"
#include "Include/Libraries/FlagTracking/FlagSorting.hpp"


FlagSorting::FlagSorting(int sourceCount, int maxLife)
: m_sourceCount{sourceCount}, m_maxLife{maxLife}, m_masterCount{sourceCount * maxLife}
{
  m_masterObjects = new sortedObjects_t[m_masterCount];
}

FlagSorting::~FlagSorting()
{
  delete[] m_masterObjects;
}



//Imports the source array and sorts it by Y into sourceArray
void FlagSorting::importSource(simpleObjects_t* flagObjects, int currentSourceCount)
{
  //Amount of objects to read. Not to exceed sourceCount
  m_currentSourceCount = currentSourceCount > m_sourceCount ? m_sourceCount : currentSourceCount;

  


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
