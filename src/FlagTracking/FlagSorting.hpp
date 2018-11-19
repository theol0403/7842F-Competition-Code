#pragma once
#include "main.h"
#include "FlagTrackingMain.hpp"

struct sortedObjects_t
{
  int objSig;
  int objX;
  int objY;
  int objWidth;
  int objHeight;
  int objCenterX;
  int objCenterY;

  bool matchFound;
  int lifeCounter;
};

class FlagSorting
{

private:

  const int m_sourceLength; //Max amount of source objects to read
  sortedObjects_t* m_sourceObjects = nullptr; //Source object container
  int m_sourceCount = 0; //Current amount of source objects. Not to exceed sourceCount

  const int m_masterLength; //Size of master array
  int m_masterCount = 0; //Amount of objects currently in master array, sorted left
  sortedObjects_t* m_masterObjects = nullptr; //Master array

  const int m_objectPosThreshold;

  const int m_maxLife; //Maximum life of objects (decay)
  const float m_emaAlpha;

  int m_exportCount = 0;
  simpleObjects_t* m_exportObjects = nullptr;


  void clearArray(sortedObjects_t*, int, int);
  void swapObjects(sortedObjects_t*, int, int);
  void sortArrayY(sortedObjects_t*, int, int);

  bool compareObjects(sortedObjects_t&, sortedObjects_t&);
  int emaCalculate(int, int, float = 1.0);

public:

  FlagSorting(int, int, float = 0.5, int = 10);
  ~FlagSorting();

  void importSource(simpleObjects_t*, int);

  void mergeObject(sortedObjects_t&, sortedObjects_t&);
  void pushObject(sortedObjects_t&, sortedObjects_t&);
  void trimObject(sortedObjects_t&);

  void mergeObjects();
  void sortMaster();

  simpleObjects_t* exportObjects();
  int exportCount();

};
