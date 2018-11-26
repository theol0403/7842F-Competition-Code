#include "VisionReading.hpp"




VisionReading::VisionReading(pros::Vision& thisVision, ObjectContainer& destContainer)
:
m_thisVision(&thisVision),
m_destContainer(&destContainer)
{
  m_visionArray = new pros::vision_object[m_destContainer->m_arrayLength];
}

VisionReading::~VisionReading()
{
  delete[] m_visionArray;
  m_visionArray = nullptr;
}


void VisionReading::resetObject(int objectNum)
{
  m_destContainer->objectArray.at(objectNum).objSig = VISION_OBJECT_ERR_SIG;
  m_destContainer->objectArray.at(objectNum).objX = 0;
  m_destContainer->objectArray.at(objectNum).objY = 0;
  m_destContainer->objectArray.at(objectNum).objWidth = 0;
  m_destContainer->objectArray.at(objectNum).objHeight = 0;
  m_destContainer->objectArray.at(objectNum).objSize = 0;
  m_destContainer->objectArray.at(objectNum).objCenterX = 0;
  m_destContainer->objectArray.at(objectNum).objCenterY = 0;
  m_destContainer->objectArray.at(objectNum).discardObject = false;
}



// Looks at vision for color, counts objects, and fills them in to master array
int VisionReading::getObjects()
{
  for(int objectNum = 0; objectNum < m_destContainer->m_arrayLength; objectNum++) //Resets vision array
  {
    m_visionArray[objectNum].signature = VISION_OBJECT_ERR_SIG;
    m_visionArray[objectNum].left_coord = 0;
    m_visionArray[objectNum].top_coord = 0;
    m_visionArray[objectNum].width = 0;
    m_visionArray[objectNum].height = 0;
    m_visionArray[objectNum].x_middle_coord = 0;
    m_visionArray[objectNum].y_middle_coord = 0;
  }

  m_destContainer->currentCount = m_thisVision->read_by_size(0, m_destContainer->m_arrayLength, m_visionArray);
  if(m_destContainer->currentCount > m_destContainer->m_arrayLength) m_destContainer->currentCount = 0; //If there are no objects pros returns a huge number

  for (int objectNum = 0; objectNum < m_destContainer->m_arrayLength; objectNum++)
  {
    if(m_visionArray[objectNum].signature == VISION_OBJECT_ERR_SIG)
    {
      resetObject(objectNum);
    }
    else
    {
      m_destContainer->objectArray.at(objectNum).objSig = m_visionArray[objectNum].signature;
      m_destContainer->objectArray.at(objectNum).objX = m_visionArray[objectNum].left_coord;
      m_destContainer->objectArray.at(objectNum).objY = m_visionArray[objectNum].top_coord;
      m_destContainer->objectArray.at(objectNum).objWidth = m_visionArray[objectNum].width;
      m_destContainer->objectArray.at(objectNum).objHeight = m_visionArray[objectNum].height;
      m_destContainer->objectArray.at(objectNum).objSize = (m_visionArray[objectNum].height + m_visionArray[objectNum].width) / 2;
      m_destContainer->objectArray.at(objectNum).objCenterX = m_visionArray[objectNum].x_middle_coord;
      m_destContainer->objectArray.at(objectNum).objCenterY = m_visionArray[objectNum].y_middle_coord;
      m_destContainer->objectArray.at(objectNum).discardObject = false;
    }
  }
  return m_destContainer->currentCount;
}



int VisionReading::filterNoise(float minSize)
{
  int discardCounter = 0;
  // loop through objects, look for size, and mark to discard
  for (int objectNum = 0; objectNum < m_destContainer->currentCount; objectNum++)
  {
    if(m_destContainer->objectArray.at(objectNum).objSize < minSize)
    {
      m_destContainer->objectArray.at(objectNum).discardObject = true;
      discardCounter++;
    }
  }
  return discardCounter;
}



int VisionReading::filterSize(float sizeThreshold)
{
  int avgCount = 0;
  int avgSize = 0;
  for(int objectNum = 0; objectNum < m_destContainer->currentCount; objectNum++)
  {
    if(!m_destContainer->objectArray.at(objectNum).discardObject)
    {
      avgCount++;
      avgSize += m_destContainer->objectArray.at(objectNum).objSize;
    }
  }
  avgSize /= avgCount;

  // upper and lower ranges for size threshold
  int heightLow = avgSize - (avgSize * sizeThreshold);
  int heightHigh = avgSize + (avgSize * sizeThreshold);

  int discardCounter = 0;
  // loop through objects, look for size, and mark to discard
  for (int objectNum = 0; objectNum < m_destContainer->currentCount; objectNum++)
  {
    int objSize = m_destContainer->objectArray.at(objectNum).objSize;
    if(objSize > heightHigh || objSize < heightLow)
    {
      m_destContainer->objectArray.at(objectNum).discardObject = true;
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
  for (int objectNum = 0; objectNum < m_destContainer->currentCount; objectNum++)
  {
    objectWidth = m_destContainer->objectArray.at(objectNum).objWidth;
    heightLow = (objectWidth * wantedProp) - (objectWidth * propThreshold);
    heightHigh = (objectWidth * wantedProp) + (objectWidth * propThreshold);

    objectHeight = m_destContainer->objectArray.at(objectNum).objHeight;

    if(objectHeight < heightLow || objectHeight > heightHigh)
    {
      m_destContainer->objectArray.at(objectNum).discardObject = true;
      discardCounter++;
    }
  }
  return discardCounter;
}



int VisionReading::discardObjects()
{
  int destNum = 0;

  for (int objectNum = 0; objectNum < m_destContainer->m_arrayLength; objectNum++)
  {
    if(m_destContainer->objectArray.at(objectNum).objSig != VISION_OBJECT_ERR_SIG && !m_destContainer->objectArray.at(destNum).discardObject)
    {
      m_destContainer->objectArray.at(destNum).objSig = m_destContainer->objectArray.at(objectNum).objSig;
      m_destContainer->objectArray.at(destNum).objX = m_destContainer->objectArray.at(objectNum).objX;
      m_destContainer->objectArray.at(destNum).objY = m_destContainer->objectArray.at(objectNum).objY;
      m_destContainer->objectArray.at(destNum).objWidth = m_destContainer->objectArray.at(objectNum).objWidth;
      m_destContainer->objectArray.at(destNum).objHeight = m_destContainer->objectArray.at(objectNum).objHeight;
      m_destContainer->objectArray.at(destNum).objSize = m_destContainer->objectArray.at(objectNum).objSize;
      m_destContainer->objectArray.at(destNum).objCenterX = m_destContainer->objectArray.at(objectNum).objCenterX;
      m_destContainer->objectArray.at(destNum).objCenterY = m_destContainer->objectArray.at(objectNum).objCenterY;
      m_destContainer->objectArray.at(destNum).discardObject = false;
      destNum++;
    }
  }

  for(int objectNum = destNum; objectNum < m_destContainer->m_arrayLength; objectNum++) //Cleans the rest of the objects
  {
    resetObject(objectNum);
  }

  m_destContainer->currentCount = destNum;
  return destNum;
}



void VisionReading::debugObjects(int objectCount)
{
  if(objectCount > m_destContainer->m_arrayLength) objectCount = m_destContainer->m_arrayLength; //Bounds Checking

  for(int objectNum = 0; objectNum < objectCount; objectNum++)
  {
    std::cout << "Object " << objectNum << " | ";
    std::cout << "Width: " << m_destContainer->objectArray.at(objectNum).objWidth << " | ";
    std::cout << "Height: " << m_destContainer->objectArray.at(objectNum).objHeight << " | ";
    std::cout << "X: " << m_destContainer->objectArray.at(objectNum).objX << " | ";
    std::cout << "Y: " << m_destContainer->objectArray.at(objectNum).objY << " | ";
    std::cout << "Size: " << m_destContainer->objectArray.at(objectNum).objSize << " | ";
    std::cout << "CenterX: " << m_destContainer->objectArray.at(objectNum).objCenterX << " | ";
    std::cout << "CenterY: " << m_destContainer->objectArray.at(objectNum).objCenterY << " | ";
    std::cout << "Sig: " << m_destContainer->objectArray.at(objectNum).objSig << " | ";
    std::cout << "Discard: " << m_destContainer->objectArray.at(objectNum).discardObject << " | ";
    std::cout << "\n";
  }

  std::cout << "ObjectCount " << m_destContainer->m_arrayLength << " | ";
  std::cout << "CurrentCount " << m_destContainer->currentCount << " | ";
  std::cout << "\n";
}


void VisionReading::debugErrorSig()
{
  for(int objectNum = 0; objectNum < m_destContainer->currentCount; objectNum++)
  {
    if(m_destContainer->objectArray.at(objectNum).objSig == VISION_OBJECT_ERR_SIG)
    {
      std::cout << "ERROR: ERR_SIG IN OBJECT " << objectNum << "\n";
      pros::delay(1000);
    }
  }
}
