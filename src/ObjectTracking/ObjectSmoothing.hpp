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

  ObjectContainer* m_sourceContainer = nullptr;
  ObjectContainer* m_destContainer = nullptr;

  std::vector<sortedObjects_t> m_sourceObjects; //Source object container
  int m_sourceCount = 0; //Current amount of source objects. Not to exceed sourceCount

  const double m_objectPosThreshold;
  const double m_emaAlpha;
  const bool m_differentVel;
  const double m_emaAlphaVel;
  const int m_lifeMax; //Maximum life of objects (decay)
  const int m_lifeThreshold; //Amount of room the objects have

  const int m_masterLength; //Size of master array
  std::vector<sortedObjects_t> m_masterObjects; //Master array
  int m_masterCount = 0; //Amount of objects currently in master array, sorted left



  void clearArray(std::vector<sortedObjects_t>&, int, int);
  void swapObjects(std::vector<sortedObjects_t>&, int, int);
  void sortArrayY(std::vector<sortedObjects_t>&, int, int);
  void sortArrayLife(std::vector<sortedObjects_t>&, int, int);

  bool compareObjects(sortedObjects_t*, sortedObjects_t*);
  double emaCalculate(double, double, double = 1.0);

  void mergeObject(sortedObjects_t&, sortedObjects_t&);
  void pushObject(sortedObjects_t&, sortedObjects_t&);
  void trimObject(sortedObjects_t&);

  void importSource();

  void mergeObjects();
  void sortMaster();


public:

  ObjectSmoothing(ObjectContainer&, ObjectContainer&, double, double, bool, double, int, int);
  ~ObjectSmoothing();

  void exportObjects(int, int);

  void smoothObjects();

};
