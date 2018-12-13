#include "objectReading.hpp"

namespace lib7842
{

  ObjectReading::ObjectReading(pros::Vision& thisVision)
  :
  m_thisVision(&thisVision)
  {
  }

  ObjectReading::~ObjectReading()
  {
  }


  // Looks at vision for color, counts objects, and fills them in to master array
  void ObjectReading::getAllObjects(lib7842::ObjectContainer &destContainer)
  {
    pros::vision_object* tempVisionArray = new pros::vision_object[destContainer.arrayLength];

    destContainer.currentCount = m_thisVision->read_by_size(0, destContainer.arrayLength, tempVisionArray);
    if(destContainer.currentCount > destContainer.arrayLength) destContainer.currentCount = 0; //If there are no objects pros returns a huge number

    for (int objectNum = 0; objectNum < destContainer.currentCount; objectNum++)
    {
      destContainer.objectArray.at(objectNum).objSig = tempVisionArray[objectNum].signature;
      destContainer.objectArray.at(objectNum).objX = tempVisionArray[objectNum].left_coord;
      destContainer.objectArray.at(objectNum).objY = tempVisionArray[objectNum].top_coord;
      destContainer.objectArray.at(objectNum).objWidth = tempVisionArray[objectNum].width;
      destContainer.objectArray.at(objectNum).objHeight = tempVisionArray[objectNum].height;
      destContainer.objectArray.at(objectNum).objArea = tempVisionArray[objectNum].height * tempVisionArray[objectNum].width;
      destContainer.objectArray.at(objectNum).objCenterX = tempVisionArray[objectNum].x_middle_coord;
      destContainer.objectArray.at(objectNum).objCenterY = tempVisionArray[objectNum].y_middle_coord;
      destContainer.objectArray.at(objectNum).discardObject = false;
    }
  }

  //TODO: Clear vision count at beginning of loop
  //So I can do getSig, getCode
  //Getcode needs a code and a dest sig

  void ObjectReading::getSigObjects(lib7842::ObjectContainer &destContainer, std::initializer_list<int> wantedSignatures)
  {
    for(int wantedSig : wantedSignatures)
    {

    }
  }

  void ObjectReading::getCodeObjects(lib7842::ObjectContainer &destContainer, std::initializer_list<codeSig_t> wantedCodes)
  {
    for(codeSig_t wantedCode : wantedCodes)
    {

    }
  }

}
