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

    int objectCount = m_thisVision->read_by_size(0, destContainer.arrayLength, tempVisionArray);
    if(objectCount > destContainer.arrayLength) objectCount = 0; //If there are no objects pros returns a huge number

    for (int objectNum = 0; objectNum < objectCount; objectNum++)
    {
      destContainer.objectArray.at(objectNum + destContainer.currentCount).objSig = tempVisionArray[objectNum].signature;
      destContainer.objectArray.at(objectNum + destContainer.currentCount).objX = tempVisionArray[objectNum].left_coord;
      destContainer.objectArray.at(objectNum + destContainer.currentCount).objY = tempVisionArray[objectNum].top_coord;
      destContainer.objectArray.at(objectNum + destContainer.currentCount).objWidth = tempVisionArray[objectNum].width;
      destContainer.objectArray.at(objectNum + destContainer.currentCount).objHeight = tempVisionArray[objectNum].height;
      destContainer.objectArray.at(objectNum + destContainer.currentCount).objArea = tempVisionArray[objectNum].height * tempVisionArray[objectNum].width;
      destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterX = tempVisionArray[objectNum].x_middle_coord;
      destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterY = tempVisionArray[objectNum].y_middle_coord;
      destContainer.objectArray.at(objectNum + destContainer.currentCount).discardObject = false;
    }

    destContainer.currentCount += objectCount;
    delete[] tempVisionArray;
  }


  void ObjectReading::getSigObjects(lib7842::ObjectContainer &destContainer, std::initializer_list<int> wantedSignatures)
  {
    pros::vision_object* tempVisionArray = new pros::vision_object[destContainer.arrayLength];

    for(const int wantedSig : wantedSignatures)
    {
      int objectCount = m_thisVision->read_by_sig(0, wantedSig, destContainer.arrayLength, tempVisionArray);
      if(objectCount > destContainer.arrayLength) objectCount = 0; //If there are no objects pros returns a huge number

      for (int objectNum = 0; objectNum < objectCount; objectNum++)
      {
        if(tempVisionArray[objectNum].signature != wantedSig) std::cout << "SIG IS NOT WHAT I WANT";
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objSig = tempVisionArray[objectNum].signature;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objX = tempVisionArray[objectNum].left_coord;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objY = tempVisionArray[objectNum].top_coord;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objWidth = tempVisionArray[objectNum].width;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objHeight = tempVisionArray[objectNum].height;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objArea = tempVisionArray[objectNum].height * tempVisionArray[objectNum].width;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterX = tempVisionArray[objectNum].x_middle_coord;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterY = tempVisionArray[objectNum].y_middle_coord;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).discardObject = false;
      }
      destContainer.currentCount += objectCount;
    }
    delete[] tempVisionArray;
  }

  void ObjectReading::getCodeObjects(lib7842::ObjectContainer &destContainer, std::vector<codeSig_t> wantedCodes)
  {
    pros::vision_object* tempVisionArray = new pros::vision_object[destContainer.arrayLength];

    for(codeSig_t &wantedCode : wantedCodes)
    {
      int objectCount = m_thisVision->read_by_code(0, wantedCode.colorCode, destContainer.arrayLength, tempVisionArray);
      if(objectCount > destContainer.arrayLength) objectCount = 0; //If there are no objects pros returns a huge number

      for (int objectNum = 0; objectNum < objectCount; objectNum++)
      {
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objSig = wantedCode.destSig;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objX = tempVisionArray[objectNum].left_coord;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objY = tempVisionArray[objectNum].top_coord;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objWidth = tempVisionArray[objectNum].width;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objHeight = tempVisionArray[objectNum].height;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objArea = tempVisionArray[objectNum].height * tempVisionArray[objectNum].width;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterX = tempVisionArray[objectNum].x_middle_coord;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterY = tempVisionArray[objectNum].y_middle_coord;
        destContainer.objectArray.at(objectNum + destContainer.currentCount).discardObject = false;
      }
      destContainer.currentCount += objectCount;
    }
    delete[] tempVisionArray;
  }


  codeSig_t ObjectReading::createCodeSig(int sig_id1, int sig_id2, int sig_id3, int sig_id4, int sig_id5)
  {
    codeSig_t tempCode;
    tempCode.destSig = sig_id1 + (NUM_SIGNATURES/2);
    tempCode.colorCode = m_thisVision->create_color_code(sig_id1, sig_id2, sig_id3, sig_id4, sig_id5);
    return tempCode;
  }


}
