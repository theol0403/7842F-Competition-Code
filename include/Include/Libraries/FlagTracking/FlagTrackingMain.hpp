#include "main.h"


extern int OBJECT_NUM;


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

  int m_objectCount{0};

  colorObjects m_flagObjects[OBJECT_NUM] = {};


public:


  visionTracking(int);

  int getObjects();
  int filterObjectSize(float sizeThreshold = 0.5);
  int filterObjectProp(float propThreshold = 0.2);
  int discardObjects();
  void exportArray(colorObjects flagExport[OBJECT_NUM])

};
