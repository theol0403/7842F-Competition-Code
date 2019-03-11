#include "objectReading.hpp"

namespace lib7842
{


  ObjectReading::ObjectReading(int port) : vision(port) {}

  void ObjectReading::getAll()
  {
    const int maxCount = 20;
    pros::vision_object* tempObjs = new pros::vision_object[maxCount];

    int count = vision.read_by_size(0, maxCount, tempObjs);
    if(count > maxCount) count = 0; //If there are no objects pros returns a huge number

    for (int objectNum = 0; objectNum < count; objectNum++) {
      visionObj newObj;
      newObj.sig = tempObjs[objectNum].signature;
      newObj.x = tempObjs[objectNum].left_coord;
      newObj.y = tempObjs[objectNum].top_coord;
      newObj.width = tempObjs[objectNum].width;
      newObj.height = tempObjs[objectNum].height;
      newObj.area = tempObjs[objectNum].width * tempObjs[objectNum].height;
      newObj.centerX = tempObjs[objectNum].x_middle_coord;
      newObj.centerY = tempObjs[objectNum].y_middle_coord;
      addObj(newObj);
    }
    delete[] tempObjs;
  }


  // void ObjectReading::getAll()
  // {
  //   const int maxCount = 20;
  //   pros::vision_object* tempObjs = new pros::vision_object[maxCount];
  //
  //   int count = vision.read_by_size(0, maxCount, tempObjs);
  //   if(count > maxCount) count = 0; //If there are no objects pros returns a huge number
  //
  //   for (int objectNum = 0; objectNum < count; objectNum++) {
  //     visionObj newObj;
  //     newObj.sig = tempObjs[objectNum].signature;
  //     newObj.x = tempObjs[objectNum].left_coord;
  //     newObj.y = tempObjs[objectNum].top_coord;
  //     newObj.width = tempObjs[objectNum].width;
  //     newObj.height = tempObjs[objectNum].height;
  //     newObj.area = tempObjs[objectNum].width * tempObjs[objectNum].height;
  //     newObj.centerX = tempObjs[objectNum].x_middle_coord;
  //     newObj.centerY = tempObjs[objectNum].y_middle_coord;
  //     addObj(newObj);
  //   }
  //   delete[] tempObjs;
  // }

  //
  // void ObjectReading::getSigObjects(lib7842::ObjectContainer &destContainer, std::initializer_list<int> wantedSignatures)
  // {
  //   pros::vision_object* tempObjs = new pros::vision_object[destContainer.arrayLength];
  //
  //   for(const int wantedSig : wantedSignatures)
  //   {
  //     int readCount = m_thisVision->read_by_sig(0, wantedSig, destContainer.arrayLength, tempObjs);
  //     if(readCount > destContainer.arrayLength) readCount = 0; //If there are no objects pros returns a huge number
  //
  //     int goodCount = 0;
  //
  //     for (int objectNum = 0; objectNum < readCount; objectNum++)
  //     {
  //       if(tempObjs[objectNum].signature != VISION_OBJECT_ERR_SIG)
  //       {
  //         if(tempObjs[objectNum].signature != wantedSig) { std::cout << "REEEE: " << tempObjs[objectNum].signature << "\n"; }
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objSig = tempObjs[objectNum].signature;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objX = tempObjs[objectNum].left_coord;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objY = tempObjs[objectNum].top_coord;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objWidth = tempObjs[objectNum].width;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objHeight = tempObjs[objectNum].height;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objArea = tempObjs[objectNum].height * tempObjs[objectNum].width;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterX = tempObjs[objectNum].x_middle_coord;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterY = tempObjs[objectNum].y_middle_coord;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).discardObject = false;
  //         goodCount++;
  //       }
  //     }
  //     destContainer.currentCount += goodCount;
  //   }
  //   delete[] tempObjs;
  // }
  //
  // void ObjectReading::getCodeObjects(lib7842::ObjectContainer &destContainer, std::vector<codeSig_t> wantedCodes)
  // {
  //   pros::vision_object* tempObjs = new pros::vision_object[destContainer.arrayLength];
  //
  //   for(codeSig_t &wantedCode : wantedCodes)
  //   {
  //     int count = m_thisVision->read_by_code(0, wantedCode.colorCode, destContainer.arrayLength, tempObjs);
  //     if(count > destContainer.arrayLength) count = 0; //If there are no objects pros returns a huge number
  //
  //     for (int objectNum = 0; objectNum < count; objectNum++)
  //     {
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objSig = wantedCode.destSig;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objX = tempObjs[objectNum].left_coord;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objY = tempObjs[objectNum].top_coord;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objWidth = tempObjs[objectNum].width;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objHeight = tempObjs[objectNum].height;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objArea = tempObjs[objectNum].height * tempObjs[objectNum].width;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterX = tempObjs[objectNum].x_middle_coord;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterY = tempObjs[objectNum].y_middle_coord;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).discardObject = false;
  //     }
  //     destContainer.currentCount += count;
  //   }
  //   delete[] tempObjs;
  // }
  //
  //
  // codeSig_t ObjectReading::createCodeSig(int sig_id1, int sig_id2, int sig_id3, int sig_id4, int sig_id5)
  // {
  //   codeSig_t tempCode;
  //   tempCode.destSig = sig_id1 + (NUM_SIGNATURES/2);
  //   tempCode.colorCode = m_thisVision->create_color_code(sig_id1, sig_id2, sig_id3, sig_id4, sig_id5);
  //   return tempCode;
  // }


}
