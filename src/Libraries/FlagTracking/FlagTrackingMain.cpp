#include "main.h"
//#include "FlagTrackingData.hpp"
//#include "FlagTrackingFunctions.hpp"


//pros::Vision mainVision(VISION_PORT);




class visionTracking
{

private:
  pros::Vision m_thisVision;

  struct flagSig_t
  {
    int Blue;
    int Red;
} const m_flagSig;

  const int m_numObjects;
  const int m_refreshRate;




  visionTracking(int portNum, int blueSig, int redSig, int numObjects = 30, int refreshRate = 30)
  :
  m_thisVision(portNum),
  m_flagSig{blueSig, redSig},
  m_numObjects{numObjects},
  m_refreshRate{refreshRate}
  {
  }


};





//
//
//
//
// //Creates struct array for storing object data
// colorObjects flagObjects[NUM_VISION_OBJECTS];
//
//
// // main task
// void flagCalculateTask(void*)
// {
//   while (true)
//   {
//     filterObjectArrays.objectcount = getObjects(); //Calculates Objects
//
//     filterObjects();
//
//     delay(VISION_REFRESH);
//   }
// }
//
//
//
//
//
// void fillObjData(vision_object* visionObjectArray, int startPos, int objCount, int objColor)
// {
//   for (int objectNum = startPos; objectNum < objCount; objectNum++)
//   {
//     flagObjects[objectNum].objY = visionObjectArray[objectNum].top_coord;
//     flagObjects[objectNum].objX = visionObjectArray[objectNum].left_coord;
//     flagObjects[objectNum].objWidth = visionObjectArray[objectNum].width;
//     flagObjects[objectNum].objHeight = visionObjectArray[objectNum].height;
//     flagObjects[objectNum].objAvgDim = (visionObjectArray[objectNum].height + visionObjectArray[objectNum].width) / 2;
//     flagObjects[objectNum].objColor = objColor;
//   }
// }
//
//
//
//
//
// // Looks at vision for color, counts objects, and fills them in
// int getObjects()
// {
//   vision_object visionObjectArray[NUM_VISION_OBJECTS]; //Creates container array for vision objects
//
//     int objCount;
//     int startPos = 0;
//
//     objCount = mainVision.read_by_sig(0, FLAG_BLUE_SIG, NUM_VISION_OBJECTS/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_BLUE_COLOR);
//     startPos = objCount;
//
//     objCount = mainVision.read_by_sig(0, FLAG_RED_SIG, NUM_VISION_OBJECTS/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_RED_COLOR);
//
//     return startPos + objCount;
//
// }
