#include "main.h"
#pragma once



struct sortedObjects_t
{
  int objSig;
  int objX;
  int objY;
  int objWidth;
  int objHeight;
  int objCenterX;
  int objCenterY;

  int lifeCounter;
  bool matchFound;
};




class FlagSorting
{

private:

const int m_sourceCount; //Max amount of source objects to read
sortedObjects_t* m_sourceObjects = nullptr; //Source object container
int m_currentSourceCount = 0; //Current amount of source objects. Not to exceed sourceCount


const int m_masterCount; //Size of master array
const int m_maxLife; //Maximum life of objects (decay)

sortedObjects_t* m_tempObjects = nullptr; //Temp array to allign objects into master

int m_currentMasterCount = 0; //Amount of objects currently in master array, sorted left
sortedObjects_t* m_masterObjects = nullptr; //Master array







public:


  FlagSorting(int, int);
  ~FlagSorting();

  void clearArray(sortedObjects_t*, int, int);
  void swapObjects(sortedObjects_t*, int, int);
  void sortArrayY(sortedObjects_t*, int);

  void importSource(simpleObjects_t*, int);


  void allignTempObjects();

  void sortMaster();



};
