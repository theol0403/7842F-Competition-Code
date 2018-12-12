#include "objectReading.hpp"

namespace lib7842
{

  ObjectReading::ObjectReading(pros::Vision& thisVision, ObjectContainer& destContainer)
  :
  m_thisVision(&thisVision),
  m_destContainer(&destContainer)
  {
    m_visionArray = new pros::vision_object[m_destContainer->arrayLength];
  }

  ObjectReading::~ObjectReading()
  {
    delete[] m_visionArray;
    m_visionArray = nullptr;
  }


  // Looks at vision for color, counts objects, and fills them in to master array
  void ObjectReading::getObjects()
  {
    // for(int objectNum = 0; objectNum < m_destContainer->arrayLength; objectNum++) //Resets vision array
    // {
    //   m_visionArray[objectNum].signature = VISION_OBJECT_ERR_SIG;
    //   m_visionArray[objectNum].left_coord = 0;
    //   m_visionArray[objectNum].top_coord = 0;
    //   m_visionArray[objectNum].width = 0;
    //   m_visionArray[objectNum].height = 0;
    //   m_visionArray[objectNum].x_middle_coord = 0;
    //   m_visionArray[objectNum].y_middle_coord = 0;
    // }

    m_destContainer->currentCount = m_thisVision->read_by_size(0, m_destContainer->arrayLength, m_visionArray);
    if(m_destContainer->currentCount > m_destContainer->arrayLength) m_destContainer->currentCount = 0; //If there are no objects pros returns a huge number

    for (int objectNum = 0; objectNum < m_destContainer->currentCount; objectNum++)
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

}
