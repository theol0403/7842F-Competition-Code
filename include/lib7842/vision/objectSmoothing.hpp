#pragma once
#include "objectTracking.hpp"

namespace lib7842
{

  struct sortedObjects_t
  {
    int objSig = VISION_OBJECT_ERR_SIG;
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

    std::array<bool, NUM_SIGNATURES+1> signaturesFound {false};
  };

  struct sigReroute_t
  {
    int sourceSig = VISION_OBJECT_ERR_SIG;
    int destSig = VISION_OBJECT_ERR_SIG;
  };

  struct compareThresh_t
  {
    int posThresh = 0;
    int dimThresh = 0;
  };

  class ObjectSmoothing
  {

  private:

    lib7842::ObjectContainer* m_sourceContainer = nullptr;
    lib7842::ObjectContainer* m_destContainer = nullptr;

    std::vector<sortedObjects_t> m_sourceObjects; //Source object container
    int m_sourceCount = 0; //Current amount of source objects. Not to exceed sourceCount

    std::vector<sigReroute_t> m_sigReroutes;

    const int m_lifeMax; //Maximum life of objects (decay)
    const int m_lifeThreshold; //Amount of room the objects have
    const int m_lifeIncrement; //Amount to increment for a sucessful merge
    const double m_emaAlpha;
    const double m_emaAlphaVel;
    std::vector<compareThresh_t> m_compareThresh;

    const int m_debugMode;

    const int m_masterLength; //Size of master array
    std::vector<sortedObjects_t> m_masterObjects; //Master array
    int m_masterCount = 0; //Amount of objects currently in master array, sorted left


    void clearArray(std::vector<sortedObjects_t>&, int, int);
    void swapObjects(std::vector<sortedObjects_t>&, int, int);
    void sortArrayY(std::vector<sortedObjects_t>&, int, int);
    void sortArrayLife(std::vector<sortedObjects_t>&, int, int);

    bool compareObjects(int, sortedObjects_t&, sortedObjects_t&);
    double emaCalculate(double, double, double = 1.0);

    void mergeObject(sortedObjects_t&, sortedObjects_t&);
    void pushObject(int, sortedObjects_t&, sortedObjects_t&);
    void trimObject(sortedObjects_t&);

    void importSource();

    void mergeObjects();
    void sortMaster();


  public:

    ObjectSmoothing(
      ObjectContainer&, ObjectContainer&,
      std::initializer_list<sigReroute_t>,
      int, int, int,
      double, double,
      std::initializer_list<compareThresh_t>,
      int = 0
    );
    ~ObjectSmoothing();

    void exportObjects(lib7842::ObjectContainer*, int, int);

    void smoothObjects();

  };

}
