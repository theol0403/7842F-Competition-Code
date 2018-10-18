#include "main.h"
//#include "FlagTrackingData.hpp"
//#include "FlagTrackingFunctions.hpp"


//pros::Vision mainVision(VISION_PORT);




class visionTracking
{

private:


  struct flagSig_t
  {
    int Blue;
    int Red;
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




  pros::Vision m_thisVision;
  const struct flagSig_t m_flagSig;

  const int m_objectNum{30};
  colorObjects m_flagObjects[30] = {};




  visionTracking(int portNum, int blueSig, int redSig)
  :
  m_thisVision(portNum),
  m_flagSig{blueSig, redSig}
  {
  }


int m_objectCount{0};



public:

  // Looks at vision for color, counts objects, and fills them in to master array
  int getObjects()
  {
    pros::vision_object visionTempArray[m_objectNum]; //Creates temp array for vision objects
    m_objectCount = m_thisVision.read_by_size(0, m_objectNum, visionTempArray);


    for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
      {
        m_flagObjects[objectNum].objSig = visionTempArray[objectNum].signature;
        m_flagObjects[objectNum].objY = visionTempArray[objectNum].top_coord;
        m_flagObjects[objectNum].objX = visionTempArray[objectNum].left_coord;
        m_flagObjects[objectNum].objWidth = visionTempArray[objectNum].width;
        m_flagObjects[objectNum].objHeight = visionTempArray[objectNum].height;
        m_flagObjects[objectNum].objSize = (visionTempArray[objectNum].height + visionTempArray[objectNum].width) / 2;
        m_flagObjects[objectNum].discardObject = false;
      }

      return m_objectCount;
  }



  int filterObjectSize(float sizeThreshold = 0.5)
    {
      int discardCounter = 0;
      int avgSize = 0;

      //Total object sizes
      for(int objectNum = 0; objectNum < m_objectCount; objectNum++)
      {
        avgSize += m_flagObjects[objectNum].objSize;
      }
      avgSize /= m_objectCount;

      // upper and lower ranges for size threshold
      int sizeLow = avgSize - (avgSize * sizeThreshold);
      int sizeHigh = avgSize + (avgSize * sizeThreshold);


      // loop through objects, look for size, and mark to discard
      for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
      {
        int objSize = m_flagObjects[objectNum].objSize;
        if(objSize > sizeHigh || objSize < sizeLow)
        {
          m_flagObjects[objectNum].discardObject = true;
          discardCounter++;
        }
        else
        {
        }

      }

      return discardCounter;
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
//     flagObjects[objectNum].objSize = (visionObjectArray[objectNum].height + visionObjectArray[objectNum].width) / 2;
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
