#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTrackingMain.hpp"




VisionReading::VisionReading(int portNum, int objectNum)
:
m_thisVision(portNum),
m_objectNum{objectNum}
{
  m_flagObjects = new simpleObjects[objectNum];
}

VisionReading::~VisionReading()
{
  delete[] m_flagObjects;
}




// Looks at vision for color, counts objects, and fills them in to master array
int VisionReading::getObjects()
{
  pros::vision_object visionTempArray[m_objectNum] = {0}; //Creates temp array for vision objects
  m_currentCount = m_thisVision.read_by_size(0, m_objectNum, visionTempArray);

  if(m_currentCount > m_objectNum) m_currentCount = 0;

  for (int objectNum = 0; objectNum < m_objectNum; objectNum++)
  {
    if(visionTempArray[objectNum].signature == VISION_OBJECT_ERR_SIG)
    {
      m_flagObjects[objectNum].objSig = VISION_OBJECT_ERR_SIG;
      m_flagObjects[objectNum].objY = 0;
      m_flagObjects[objectNum].objX = 0;
      m_flagObjects[objectNum].objWidth = 0;
      m_flagObjects[objectNum].objHeight = 0;
      m_flagObjects[objectNum].objSize = 0;
      m_flagObjects[objectNum].objCenterX = 0;
      m_flagObjects[objectNum].objCenterY = 0;
      m_flagObjects[objectNum].discardObject = false;
    }
    else
    {
      m_flagObjects[objectNum].objSig = visionTempArray[objectNum].signature;
      m_flagObjects[objectNum].objY = visionTempArray[objectNum].top_coord;
      m_flagObjects[objectNum].objX = visionTempArray[objectNum].left_coord;
      m_flagObjects[objectNum].objWidth = visionTempArray[objectNum].width;
      m_flagObjects[objectNum].objHeight = visionTempArray[objectNum].height;
      m_flagObjects[objectNum].objSize = (visionTempArray[objectNum].height + visionTempArray[objectNum].width) / 2;
      m_flagObjects[objectNum].objCenterX = visionTempArray[objectNum].x_middle_coord;
      m_flagObjects[objectNum].objCenterY = visionTempArray[objectNum].y_middle_coord;
      m_flagObjects[objectNum].discardObject = false;
    }
  }
  return m_currentCount;
}



int VisionReading::filterNoise(float minSize)
{
  int discardCounter = 0;
  // loop through objects, look for size, and mark to discard
  for (int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {
    if(m_flagObjects[objectNum].objSize < minSize)
    {
      m_flagObjects[objectNum].discardObject = true;
      discardCounter++;
    }
  }
  return discardCounter;
}



int VisionReading::filterObjectSize(float sizeThreshold)
{
  int avgCount = 0;
  int avgSize = 0;
  for(int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {
    if(!m_flagObjects[objectNum].discardObject)
    {
      avgCount++;
      avgSize += m_flagObjects[objectNum].objSize;
    }
  }
  avgSize /= avgCount;

  // upper and lower ranges for size threshold
  int heightLow = avgSize - (avgSize * sizeThreshold);
  int heightHigh = avgSize + (avgSize * sizeThreshold);

  int discardCounter = 0;
  // loop through objects, look for size, and mark to discard
  for (int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {
    int objSize = m_flagObjects[objectNum].objSize;
    if(objSize > heightHigh || objSize < heightLow)
    {
      m_flagObjects[objectNum].discardObject = true;
      discardCounter++;
    }
  }
  return discardCounter;
}







// Filters object proportions
int VisionReading::filterObjectProp(float propThreshold, float wantedProp)
{


  int objectWidth;
  int objectHeight;

  int heightLow;
  int heightHigh;


  // loop through objects, look for size, and fill into new array
  int discardCounter = 0;
  for (int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {

    objectWidth = m_flagObjects[objectNum].objWidth;
    heightLow = (objectWidth/wantedProp) - (objectWidth * propThreshold);
    heightHigh = (objectWidth/wantedProp) + (objectWidth * propThreshold);

    objectHeight = m_flagObjects[objectNum].objHeight;

    if(objectHeight < heightLow || objectWidth > heightHigh)
    {
      m_flagObjects[objectNum].discardObject = true;
      discardCounter++;
    }

  }

  return discardCounter;
}







int VisionReading::discardObjects()
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
      m_flagObjects[exportNum].objCenterX = m_flagObjects[objectNum].objCenterX;
      m_flagObjects[exportNum].objCenterY = m_flagObjects[objectNum].objCenterY;
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
      m_flagObjects[exportNum].objCenterX = 0;
      m_flagObjects[exportNum].objCenterY = 0;
      m_flagObjects[exportNum].discardObject = false;
    }

  }

  m_currentCount = exportNum + 1;
  return exportNum + 1;
}





simpleObjects* VisionReading::exportObjects()
{
  return m_flagObjects;
}


void VisionReading::debugObjects(int objectCount)
{
  for(int objectNum = 0, objectNum < objectCount, objectNum++)
  {
    std::cout << "Object " << objectNum << " | ";
    std::cout << "Width: " << m_flagObjects[objectNum].objWidth << " | ";
    std::cout << "Height: " << m_flagObjects[objectNum].objHeight << " | ";
    std::cout << "X: " << m_flagObjects[objectNum].objX << " | ";
    std::cout << "Y: " << m_flagObjects[objectNum].objY << " | ";
    std::cout << "CenterX: " << m_flagObjects[objectNum].objCenterX << " | ";
    std::cout << "CenterY: " << m_flagObjects[objectNum].objCenterY << " | ";
    std::cout << "Sig: " << m_flagObjects[objectNum].objSig << " | ";
    std::cout << "Discard: " << m_flagObjects[objectNum].discardObject << " | ";
    std::cout << "\n";
  }

  std::cout << "CurrentCount " << m_currentCount << " | ";
  std::cout << "\n";

}


void VisionReading::debugErrorSig()
{
  for(int objectNum = 0, objectNum < m_currentCount, objectNum++)
  {
    if(m_flagObjects[objectNum].objSig == VISION_OBJECT_ERR_SIG)
    {
      std::cout << "ERROR: ERR_SIG IN OBJECT " << objectNum << "\n";
      pros::delay(1000);
    }
  }
}



















//
//
// // main task
// void mainFlagTrackingTask(void*ignore)
// {
//   VisionReading mainVisionReading(9);
//   screenDrawing mainScreenDrawing(OBJECT_CONTAINER_WIDTH, OBJECT_CONTAINER_HEIGHT, OBJECT_SCALE_WIDTH, OBJECT_SCALE_HEIGHT);
//
//   while(true)
//   {
//     mainVisionReading.getObjects(); //Calculates Objects
//
//     mainVisionReading.filterObjectSize();
//     mainVisionReading.filterObjectProp();
//
//     mainVisionReading.exportObjects();
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
// simpleObjects flagObjects[m_objectNum];
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
