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


const int m_masterCount;
const int m_maxLife;
sortedObjects_t* m_masterObjects = nullptr;


const int m_sourceCount; //Amount of source objects to read to
sortedObjects_t* m_sourceObjects = nullptr;

int m_currentSourceCount; //Current amount of source objects. Not to exceed sourceCount


public:


  FlagSorting(int, int);
  ~FlagSorting();

  void importSource(simpleObjects_t*, int);

  void sortMaster();



};
