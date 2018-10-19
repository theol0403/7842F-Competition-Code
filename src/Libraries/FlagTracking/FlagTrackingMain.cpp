#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"





class visionTracking
{

private:


  pros::Vision m_thisVision;


public:

  visionTracking(int portNum)
  :
  m_thisVision(portNum)
  {
  }

  int m_objectCount{0};

  const int m_objectNum{OBJECT_NUM};
  colorObjects m_flagObjects[OBJECT_NUM] = {};



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







  // Filters object proportions
  int filterObjectProp(float propThreshold = 0.2)
  {
    int discardCounter = 0;

    int sizeWidth;
    int sizeHeight;
    int objectSize;
    int sizeLow;
    int sizeHigh;


    // loop through objects, look for size, and fill into new array
    for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
    {

      objectSize = m_flagObjects[objectNum].objSize;
      sizeLow = objectSize - (objectSize * propThreshold);
      sizeHigh = objectSize + (objectSize * propThreshold);

      sizeWidth = m_flagObjects[objectNum].objWidth;
      sizeHeight = m_flagObjects[objectNum].objHeight;


      if (sizeWidth < sizeLow || sizeWidth > sizeHigh || sizeHeight < sizeLow || sizeWidth > sizeHigh)
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





  colorObjects * exportArray()
  {
    static colorObjects flagExport[OBJECT_NUM];

    int exportNum = 0;

    for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
    {
      if(m_flagObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG && !m_flagObjects[objectNum].discardObject)
      {
        flagExport[exportNum].objSig = m_flagObjects[objectNum].objSig;
        flagExport[exportNum].objY = m_flagObjects[objectNum].objY;
        flagExport[exportNum].objX = m_flagObjects[objectNum].objX;
        flagExport[exportNum].objWidth = m_flagObjects[objectNum].objWidth;
        flagExport[exportNum].objHeight = m_flagObjects[objectNum].objHeight;
        flagExport[exportNum].objSize = m_flagObjects[objectNum].objSize;
        flagExport[exportNum].discardObject = false;
        exportNum++;
      }
      else
      {
        flagExport[exportNum].objSig = VISION_OBJECT_ERR_SIG;
        flagExport[exportNum].objY = 0;
        flagExport[exportNum].objX = 0;
        flagExport[exportNum].objWidth = 0;
        flagExport[exportNum].objHeight = 0;
        flagExport[exportNum].objSize = 0;
        flagExport[exportNum].discardObject = false;
      }

    }
    return flagExport;
  }






};








// main task
void mainFlagTrackingTask(void*ignore)
{
  visionTracking mainVisionTracking(9);

  while(true)
  {
    mainVisionTracking.getObjects(); //Calculates Objects

    mainVisionTracking.filterObjectSize();
    mainVisionTracking.filterObjectProp();

    mainScreenDrawing.drawFlagObjects(mainVisionTracking.exportArray());




    pros::delay(100);
  }
}



















//
//
//
//
// //Creates struct array for storing object data
// colorObjects flagObjects[OBJECT_NUM];
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
//   vision_object visionObjectArray[OBJECT_NUM]; //Creates container array for vision objects
//
//     int objCount;
//     int startPos = 0;
//
//     objCount = mainVision.read_by_sig(0, FLAG_BLUE_SIG, OBJECT_NUM/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_BLUE_COLOR);
//     startPos = objCount;
//
//     objCount = mainVision.read_by_sig(0, FLAG_RED_SIG, OBJECT_NUM/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_RED_COLOR);
//
//     return startPos + objCount;
//
// }
