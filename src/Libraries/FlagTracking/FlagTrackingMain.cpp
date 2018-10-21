#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"




visionTracking::visionTracking(int portNum, int objectNum)
:
m_thisVision(portNum),
m_objectNum(objectNum)
{
  m_flagObjects = new visionObjects[objectNum];
}


visionTracking::~visionTracking()
{
  delete[] m_flagObjects;
}


// Looks at vision for color, counts objects, and fills them in to master array
int visionTracking::getObjects()
{
  pros::vision_object visionTempArray[m_objectNum]; //Creates temp array for vision objects
  m_currentCount = m_thisVision.read_by_size(0, m_objectNum, visionTempArray);


  for (int objectNum = 0; objectNum < m_objectNum; objectNum++)
  {
    m_flagObjects[objectNum].objSig = visionTempArray[objectNum].signature;
    m_flagObjects[objectNum].objY = visionTempArray[objectNum].top_coord;
    m_flagObjects[objectNum].objX = visionTempArray[objectNum].left_coord;
    m_flagObjects[objectNum].objWidth = visionTempArray[objectNum].width;
    m_flagObjects[objectNum].objHeight = visionTempArray[objectNum].height;
    m_flagObjects[objectNum].objSize = (visionTempArray[objectNum].height + visionTempArray[objectNum].width) / 2;
    m_flagObjects[objectNum].discardObject = false;
  }

  return m_currentCount;
}



int visionTracking::filterObjectSize(float sizeThreshold)
{
  int discardCounter = 0;
  int avgSize = 0;

  //Total object sizes
  for(int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {
    avgSize += m_flagObjects[objectNum].objSize;
  }
  avgSize /= m_objectNum;

  // upper and lower ranges for size threshold
  int sizeLow = avgSize - (avgSize * sizeThreshold);
  int sizeHigh = avgSize + (avgSize * sizeThreshold);


  // loop through objects, look for size, and mark to discard
  for (int objectNum = 0; objectNum < m_currentCount; objectNum++)
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
int visionTracking::filterObjectProp(float propThreshold)
{
  int discardCounter = 0;

  int sizeWidth;
  int sizeHeight;
  int objectSize;
  int sizeLow;
  int sizeHigh;


  // loop through objects, look for size, and fill into new array
  for (int objectNum = 0; objectNum < m_currentCount; objectNum++)
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







int visionTracking::discardObjects()
{
  int exportNum = 0;

  for (int objectNum = 0; objectNum < m_objectNum; objectNum++)
  {
    if(m_flagObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG && !m_flagObjects[exportNum].discardObject)
    {
      m_flagObjects[exportNum].objSig = m_flagObjects[objectNum].objSig;
      m_flagObjects[exportNum].objY = m_flagObjects[objectNum].objY;
      m_flagObjects[exportNum].objX = m_flagObjects[objectNum].objX;
      m_flagObjects[exportNum].objWidth = m_flagObjects[objectNum].objWidth;
      m_flagObjects[exportNum].objHeight = m_flagObjects[objectNum].objHeight;
      m_flagObjects[exportNum].objSize = m_flagObjects[objectNum].objSize;
      m_flagObjects[exportNum].discardObject = false;
      exportNum++;
    }
    else
    {
      m_flagObjects[exportNum].objSig = VISION_OBJECT_ERR_SIG;
      m_flagObjects[exportNum].objY = 0;
      m_flagObjects[exportNum].objX = 0;
      m_flagObjects[exportNum].objWidth = 0;
      m_flagObjects[exportNum].objHeight = 0;
      m_flagObjects[exportNum].objSize = 0;
      m_flagObjects[exportNum].discardObject = false;
    }

  }

  m_currentCount = exportNum + 1;
  return exportNum + 1;
}





visionObjects* visionTracking::exportObjects()
{
  return m_flagObjects;
}





















//
//
// // main task
// void mainFlagTrackingTask(void*ignore)
// {
//   visionTracking mainVisionTracking(9);
//   screenDrawing mainScreenDrawing(OBJECT_CONTAINER_WIDTH, OBJECT_CONTAINER_HEIGHT, OBJECT_SCALE_WIDTH, OBJECT_SCALE_HEIGHT);
//
//   while(true)
//   {
//     mainVisionTracking.getObjects(); //Calculates Objects
//
//     mainVisionTracking.filterObjectSize();
//     mainVisionTracking.filterObjectProp();
//
//     mainVisionTracking.exportObjects();
//     mainScreenDrawing.drawFlagObjects();
//
//
//
//
//     pros::delay(100);
//   }
// }
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//


//
//
//
//
// //Creates struct array for storing object data
// visionObjects flagObjects[m_objectNum];
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
//   vision_object visionObjectArray[m_objectNum]; //Creates container array for vision objects
//
//     int objCount;
//     int startPos = 0;
//
//     objCount = mainVision.read_by_sig(0, FLAG_BLUE_SIG, m_objectNum/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_BLUE_COLOR);
//     startPos = objCount;
//
//     objCount = mainVision.read_by_sig(0, FLAG_RED_SIG, m_objectNum/2, visionObjectArray);
//     fillObjData(visionObjectArray, startPos, objCount, OBJ_RED_COLOR);
//
//     return startPos + objCount;
//
// }
