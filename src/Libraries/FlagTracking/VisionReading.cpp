#include "main.h"

#include "Include/Libraries/FlagTracking/FlagTracking.hpp"
#include "Include/Libraries/FlagTracking/VisionReading.hpp"




VisionReading::VisionReading(int portNum, int objectCount)
: m_thisVision(portNum), m_objectCount{objectCount}
{
  m_visionArray = new pros::vision_object[objectCount];
  m_flagObjects = new simpleObjects_t[objectCount];
}

VisionReading::~VisionReading()
{
  delete[] m_visionArray;
  m_visionArray = nullptr;
  delete[] m_flagObjects;
  m_flagObjects = nullptr;
}


void VisionReading::resetObject(int objectNum)
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



// Looks at vision for color, counts objects, and fills them in to master array
int VisionReading::getObjects()
{
  for(int objectNum = 0; objectNum < m_objectCount; objectNum++) //Resets vision array
  {
    m_visionArray[objectNum].signature = VISION_OBJECT_ERR_SIG;
    m_visionArray[objectNum].top_coord = 0;
    m_visionArray[objectNum].left_coord = 0;
    m_visionArray[objectNum].width = 0;
    m_visionArray[objectNum].height = 0;
    m_visionArray[objectNum].x_middle_coord = 0;
    m_visionArray[objectNum].y_middle_coord = 0;
  }

  m_currentCount = m_thisVision.read_by_size(0, m_objectCount, m_visionArray);
  if(m_currentCount > m_objectCount) m_currentCount = 0; //If there are no objects pros returns a huge number

  for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
  {
    if(m_visionArray[objectNum].signature == VISION_OBJECT_ERR_SIG)
    {
      resetObject(objectNum);
    }
    else
    {
      m_flagObjects[objectNum].objSig = m_visionArray[objectNum].signature;
      m_flagObjects[objectNum].objY = m_visionArray[objectNum].top_coord;
      m_flagObjects[objectNum].objX = m_visionArray[objectNum].left_coord;
      m_flagObjects[objectNum].objWidth = m_visionArray[objectNum].width;
      m_flagObjects[objectNum].objHeight = m_visionArray[objectNum].height;
      m_flagObjects[objectNum].objSize = (m_visionArray[objectNum].height + m_visionArray[objectNum].width) / 2;
      m_flagObjects[objectNum].objCenterX = m_visionArray[objectNum].x_middle_coord;
      m_flagObjects[objectNum].objCenterY = m_visionArray[objectNum].y_middle_coord;
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



int VisionReading::filterSize(float sizeThreshold)
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
int VisionReading::filterProp(float propThreshold, float wantedProp)
{
  int objectWidth;
  int objectHeight;

  int heightLow;
  int heightHigh;

  // loop through objects, look for proportion, and fill into new array
  int discardCounter = 0;
  for (int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {
    objectWidth = m_flagObjects[objectNum].objWidth;
    heightLow = (objectWidth * wantedProp) - (objectWidth * propThreshold);
    heightHigh = (objectWidth * wantedProp) + (objectWidth * propThreshold);

    objectHeight = m_flagObjects[objectNum].objHeight;

    if(objectHeight < heightLow || objectHeight > heightHigh)
    {
      m_flagObjects[objectNum].discardObject = true;
      discardCounter++;
    }
  }
  return discardCounter;
}







int VisionReading::discardObjects()
{
  int destNum = 0;

  for (int objectNum = 0; objectNum < m_objectCount; objectNum++)
  {
    if(m_flagObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG && !m_flagObjects[destNum].discardObject)
    {
      m_flagObjects[destNum].objSig = m_flagObjects[objectNum].objSig;
      m_flagObjects[destNum].objY = m_flagObjects[objectNum].objY;
      m_flagObjects[destNum].objX = m_flagObjects[objectNum].objX;
      m_flagObjects[destNum].objWidth = m_flagObjects[objectNum].objWidth;
      m_flagObjects[destNum].objHeight = m_flagObjects[objectNum].objHeight;
      m_flagObjects[destNum].objSize = m_flagObjects[objectNum].objSize;
      m_flagObjects[destNum].objCenterX = m_flagObjects[objectNum].objCenterX;
      m_flagObjects[destNum].objCenterY = m_flagObjects[objectNum].objCenterY;
      m_flagObjects[destNum].discardObject = false;
      destNum++;
    }
  }

  for(int objectNum = destNum; objectNum < m_objectCount; objectNum++) //Cleans the rest of the objects
  {
    resetObject(objectNum);
  }

  m_currentCount = destNum;
  return destNum;
}





simpleObjects_t* VisionReading::exportObjects()
{
  return m_flagObjects;
}


int VisionReading::exportCurrentCount()
{
  return m_currentCount;
}


void VisionReading::debugObjects(int objectCount)
{
  if(objectCount > m_objectCount) objectCount = m_objectCount; //Bounds Checking

  for(int objectNum = 0; objectNum < objectCount; objectNum++)
  {
    std::cout << "Object " << objectNum << " | ";
    std::cout << "Width: " << m_flagObjects[objectNum].objWidth << " | ";
    std::cout << "Height: " << m_flagObjects[objectNum].objHeight << " | ";
    std::cout << "X: " << m_flagObjects[objectNum].objX << " | ";
    std::cout << "Y: " << m_flagObjects[objectNum].objY << " | ";
    std::cout << "Size: " << m_flagObjects[objectNum].objSize << " | ";
    std::cout << "CenterX: " << m_flagObjects[objectNum].objCenterX << " | ";
    std::cout << "CenterY: " << m_flagObjects[objectNum].objCenterY << " | ";
    std::cout << "Sig: " << m_flagObjects[objectNum].objSig << " | ";
    std::cout << "Discard: " << m_flagObjects[objectNum].discardObject << " | ";
    std::cout << "\n";
  }

  std::cout << "ObjectCount " << m_objectCount << " | ";
  std::cout << "CurrentCount " << m_currentCount << " | ";
  std::cout << "\n";
}


void VisionReading::debugErrorSig()
{
  for(int objectNum = 0; objectNum < m_currentCount; objectNum++)
  {
    if(m_flagObjects[objectNum].objSig == VISION_OBJECT_ERR_SIG)
    {
      std::cout << "ERROR: ERR_SIG IN OBJECT " << objectNum << "\n";
      pros::delay(1000);
    }
  }
}
