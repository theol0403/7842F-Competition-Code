#include "objectReading.hpp"

namespace lib7842
{


  ObjectReading::ObjectReading(int port) : vision(port), maxCount(20) {}

  void ObjectReading::getAll()
  {
    pros::vision_object* temp = new pros::vision_object[maxCount];

    int count = vision.read_by_size(0, maxCount, temp);
    if(count > maxCount) count = 0; //If there are no objects pros returns a huge number

    for (int objectNum = 0; objectNum < count; objectNum++) {
      visionObj newObj;
      newObj.sig = temp[objectNum].signature;
      newObj.x = temp[objectNum].left_coord;
      newObj.y = temp[objectNum].top_coord;
      newObj.width = temp[objectNum].width;
      newObj.height = temp[objectNum].height;
      newObj.area = temp[objectNum].width * temp[objectNum].height;
      newObj.centerX = temp[objectNum].x_middle_coord;
      newObj.centerY = temp[objectNum].y_middle_coord;
      addObj(newObj);
    }
    delete[] temp;
  }


  // void ObjectReading::getAll()
  // {
  //   const int maxCount = 20;
  //   pros::vision_object* temp = new pros::vision_object[maxCount];
  //
  //   int count = vision.read_by_size(0, maxCount, temp);
  //   if(count > maxCount) count = 0; //If there are no objects pros returns a huge number
  //
  //   for (int objectNum = 0; objectNum < count; objectNum++) {
  //     visionObj newObj;
  //     newObj.sig = temp[objectNum].signature;
  //     newObj.x = temp[objectNum].left_coord;
  //     newObj.y = temp[objectNum].top_coord;
  //     newObj.width = temp[objectNum].width;
  //     newObj.height = temp[objectNum].height;
  //     newObj.area = temp[objectNum].width * temp[objectNum].height;
  //     newObj.centerX = temp[objectNum].x_middle_coord;
  //     newObj.centerY = temp[objectNum].y_middle_coord;
  //     addObj(newObj);
  //   }
  //   delete[] temp;
  // }

  //
  // void ObjectReading::getSigObjects(lib7842::ObjectContainer &destContainer, std::initializer_list<int> wantedSignatures)
  // {
  //   pros::vision_object* temp = new pros::vision_object[destContainer.arrayLength];
  //
  //   for(const int wantedSig : wantedSignatures)
  //   {
  //     int readCount = m_thisVision->read_by_sig(0, wantedSig, destContainer.arrayLength, temp);
  //     if(readCount > destContainer.arrayLength) readCount = 0; //If there are no objects pros returns a huge number
  //
  //     int goodCount = 0;
  //
  //     for (int objectNum = 0; objectNum < readCount; objectNum++)
  //     {
  //       if(temp[objectNum].signature != VISION_OBJECT_ERR_SIG)
  //       {
  //         if(temp[objectNum].signature != wantedSig) { std::cout << "REEEE: " << temp[objectNum].signature << "\n"; }
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objSig = temp[objectNum].signature;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objX = temp[objectNum].left_coord;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objY = temp[objectNum].top_coord;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objWidth = temp[objectNum].width;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objHeight = temp[objectNum].height;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objArea = temp[objectNum].height * temp[objectNum].width;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterX = temp[objectNum].x_middle_coord;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterY = temp[objectNum].y_middle_coord;
  //         destContainer.objectArray.at(objectNum + destContainer.currentCount).discardObject = false;
  //         goodCount++;
  //       }
  //     }
  //     destContainer.currentCount += goodCount;
  //   }
  //   delete[] temp;
  // }
  //
  // void ObjectReading::getCodeObjects(lib7842::ObjectContainer &destContainer, std::vector<codeSig_t> wantedCodes)
  // {
  //   pros::vision_object* temp = new pros::vision_object[destContainer.arrayLength];
  //
  //   for(codeSig_t &wantedCode : wantedCodes)
  //   {
  //     int count = m_thisVision->read_by_code(0, wantedCode.colorCode, destContainer.arrayLength, temp);
  //     if(count > destContainer.arrayLength) count = 0; //If there are no objects pros returns a huge number
  //
  //     for (int objectNum = 0; objectNum < count; objectNum++)
  //     {
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objSig = wantedCode.destSig;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objX = temp[objectNum].left_coord;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objY = temp[objectNum].top_coord;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objWidth = temp[objectNum].width;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objHeight = temp[objectNum].height;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objArea = temp[objectNum].height * temp[objectNum].width;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterX = temp[objectNum].x_middle_coord;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).objCenterY = temp[objectNum].y_middle_coord;
  //       destContainer.objectArray.at(objectNum + destContainer.currentCount).discardObject = false;
  //     }
  //     destContainer.currentCount += count;
  //   }
  //   delete[] temp;
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
