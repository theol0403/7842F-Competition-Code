#pragma once
#include "ObjectTrackingMain.hpp"

struct sortedObjects_t
{
  int objSig = 0;
  double objX = 0;
  double objY = 0;
  double objWidth = 0;
  double objHeight = 0;
  double objCenterX = 0;
  double objCenterY = 0;
  double objXVel = 0;
  double objYVel = 0;

  bool matchFound = false;
  int lifeCounter = 0;
};

class ObjectSmoothing
{

private:

  const int m_sourceLength; //Max amount of source objects to read
  sortedObjects_t* m_sourceObjects = nullptr; //Source object container
  int m_sourceCount = 0; //Current amount of source objects. Not to exceed sourceCount

  const double m_objectPosThreshold;
  const double m_emaAlpha;
  const bool m_differentVel;
  const double m_emaAlphaVel;
  const int m_lifeMax; //Maximum life of objects (decay)
  const int m_lifeThreshold; //Amount of room the objects have

  const int m_masterLength; //Size of master array
  sortedObjects_t* m_masterObjects = nullptr; //Master array
  int m_masterCount = 0; //Amount of objects currently in master array, sorted left

  const bool m_debugInfo = false;


  void clearArray(sortedObjects_t*, int, int);
  void swapObjects(sortedObjects_t*, int, int);
  void sortArrayY(sortedObjects_t*, int, int);
  void sortArrayLife(sortedObjects_t*, int, int);

  bool compareObjects(sortedObjects_t*, sortedObjects_t*);
  double emaCalculate(double, double, double = 1.0);

  void mergeObject(sortedObjects_t&, sortedObjects_t&);
  void pushObject(sortedObjects_t&, sortedObjects_t&);
  void trimObject(sortedObjects_t&);

  void importSource(objectContainer_t*);

  void mergeObjects();
  void sortMaster();


public:

  ObjectSmoothing(int, double, double, bool, double, int, int, bool = false);
  ~ObjectSmoothing();

  void exportObjects(objectContainer_t*, int, int);

  void smoothObjects(objectContainer_t*, objectContainer_t*);

};
