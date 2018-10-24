#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTracking.hpp"
#include "Include/Libraries/FlagTracking/FlagSorting.hpp"


FlagSorting::FlagSorting(int sourceCount, int decayTime)
: m_sourceCount{sourceCount}, m_decayTime{decayTime}, m_masterCount{sourceCount * decayTime}
{
  m_masterObjects = new sortedObjects_t[sourceCount * decayTime];
}

FlagSorting::~FlagSorting()
{
  delete[] m_masterObjects;
}





void importSource(int sourceCount); //Imports the source array and sorts it by Y


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
