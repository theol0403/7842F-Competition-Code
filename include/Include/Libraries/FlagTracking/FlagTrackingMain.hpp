#include "main.h"




struct flagSig_t
{
  int blueSig;
  int redSig;
};




struct colorObjects
{
  int objSig;
  int objX;
  int objY;
  int objWidth;
  int objHeight;
  int objSize; // Avg of width and height
  bool discardObject;
};





class visionTracking
{

private:

  pros::Vision m_thisVision;

  const int m_objectNum;

  int m_currentCount{0};

  colorObjects *m_flagObjects;


public:


  visionTracking(int, int);
  ~visionTracking();

  int getObjects();
  int filterObjectSize(float sizeThreshold = 0.5);
  int filterObjectProp(float propThreshold = 0.2);
  int discardObjects();
  colorObjects* exportArray();

};
