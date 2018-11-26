#include "VisionReading.hpp"




VisionReading::VisionReading(pros::Vision&, ObjectContainer&)
: m_thisVision(portNum), m_objectCount{objectCount}
{
  m_visionArray = new pros::vision_object[objectCount];
  m_rawObjects = new simpleObjects_t[objectCount];
}

VisionReading::~VisionReading()
{
  delete[] m_visionArray;
  m_visionArray = nullptr;
  delete[] m_rawObjects;
  m_rawObjects = nullptr;
}


void VisionReading::resetObject(int objectNum)
{
  m_rawObjects[objectNum].objSig = VISION_OBJECT_ERR_SIG;
  m_rawObjects[objectNum].objX = 0;
  m_rawObjects[objectNum].objY = 0;
  m_rawObjects[objectNum].objWidth = 0;
  m_rawObjects[objectNum].objHeight = 0;
  m_rawObjects[objectNum].objSize = 0;
  m_rawObjects[objectNum].objCenterX = 0;
  m_rawObjects[objectNum].objCenterY = 0;
  m_rawObjects[objectNum].discardObject = false;
}



// Looks at vision for color, counts objects, and fills them in to master array
int VisionReading::getObjects()
{
  for(int objectNum = 0; objectNum < m_objectCount; objectNum++) //Resets vision array
  {
    m_visionArray[objectNum].signature = VISION_OBJECT_ERR_SIG;
    m_visionArray[objectNum].left_coord = 0;
    m_visionArray[objectNum].top_coord = 0;
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
      m_rawObjects[objectNum].objSig = m_visionArray[objectNum].signature;
      m_rawObjects[objectNum].objX = m_visionArray[objectNum].left_coord;
      m_rawObjects[objectNum].objY = m_visionArray[objectNum].top_coord;
      m_rawObjects[objectNum].objWidth = m_visionArray[objectNum].width;
      m_rawObjects[objectNum].objHeight = m_visionArray[objectNum].height;
      m_rawObjects[objectNum].objSize = (m_visionArray[objectNum].height + m_visionArray[objectNum].width) / 2;
      m_rawObjects[objectNum].objCenterX = m_visionArray[objectNum].x_middle_coord;
      m_rawObjects[objectNum].objCenterY = m_visionArray[objectNum].y_middle_coord;
      m_rawObjects[objectNum].discardObject = false;
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
    if(m_rawObjects[objectNum].objSize < minSize)
    {
      m_rawObjects[objectNum].discardObject = true;
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
    if(!m_rawObjects[objectNum].discardObject)
    {
      avgCount++;
      avgSize += m_rawObjects[objectNum].objSize;
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
    int objSize = m_rawObjects[objectNum].objSize;
    if(objSize > heightHigh || objSize < heightLow)
    {
      m_rawObjects[objectNum].discardObject = true;
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
    objectWidth = m_rawObjects[objectNum].objWidth;
    heightLow = (objectWidth * wantedProp) - (objectWidth * propThreshold);
    heightHigh = (objectWidth * wantedProp) + (objectWidth * propThreshold);

    objectHeight = m_rawObjects[objectNum].objHeight;

    if(objectHeight < heightLow || objectHeight > heightHigh)
    {
      m_rawObjects[objectNum].discardObject = true;
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
    if(m_rawObjects[objectNum].objSig != VISION_OBJECT_ERR_SIG && !m_rawObjects[destNum].discardObject)
    {
      m_rawObjects[destNum].objSig = m_rawObjects[objectNum].objSig;
      m_rawObjects[destNum].objX = m_rawObjects[objectNum].objX;
      m_rawObjects[destNum].objY = m_rawObjects[objectNum].objY;
      m_rawObjects[destNum].objWidth = m_rawObjects[objectNum].objWidth;
      m_rawObjects[destNum].objHeight = m_rawObjects[objectNum].objHeight;
      m_rawObjects[destNum].objSize = m_rawObjects[objectNum].objSize;
      m_rawObjects[destNum].objCenterX = m_rawObjects[objectNum].objCenterX;
      m_rawObjects[destNum].objCenterY = m_rawObjects[objectNum].objCenterY;
      m_rawObjects[destNum].discardObject = false;
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



void VisionReading::exportObjects(objectContainer_t* exportContainer)
{
  exportContainer->objectCount = m_currentCount;
  exportContainer->objectArray = m_rawObjects;
}

void VisionReading::debugObjects(int objectCount)
{
  if(objectCount > m_objectCount) objectCount = m_objectCount; //Bounds Checking

  for(int objectNum = 0; objectNum < objectCount; objectNum++)
  {
    std::cout << "Object " << objectNum << " | ";
    std::cout << "Width: " << m_rawObjects[objectNum].objWidth << " | ";
    std::cout << "Height: " << m_rawObjects[objectNum].objHeight << " | ";
    std::cout << "X: " << m_rawObjects[objectNum].objX << " | ";
    std::cout << "Y: " << m_rawObjects[objectNum].objY << " | ";
    std::cout << "Size: " << m_rawObjects[objectNum].objSize << " | ";
    std::cout << "CenterX: " << m_rawObjects[objectNum].objCenterX << " | ";
    std::cout << "CenterY: " << m_rawObjects[objectNum].objCenterY << " | ";
    std::cout << "Sig: " << m_rawObjects[objectNum].objSig << " | ";
    std::cout << "Discard: " << m_rawObjects[objectNum].discardObject << " | ";
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
    if(m_rawObjects[objectNum].objSig == VISION_OBJECT_ERR_SIG)
    {
      std::cout << "ERROR: ERR_SIG IN OBJECT " << objectNum << "\n";
      pros::delay(1000);
    }
  }
}
