#include "objectReading.hpp"

namespace lib7842
{


  ObjectReading::ObjectReading(int port) : vision(port), maxCount(20) {}

  void ObjectReading::getAll()
  {
    std::vector<pros::vision_object> temp;
    temp.resize(maxCount);

    int count = vision.read_by_size(0, maxCount, temp.data());
    if(count > maxCount) count = 0; //If there are no objects pros returns a huge number

    for (int i = 0; i < count; i++) addObj(temp.at(i));
  }


  // void ObjectReading::getAll()
  // {
  //   const int maxCount = 20;
  //   pros::vision_object* temp = new pros::vision_object[maxCount];
  //
  //   int count = vision.read_by_size(0, maxCount, temp);
  //   if(count > maxCount) count = 0; //If there are no objects pros returns a huge number
  //
  //   for (int i = 0; i < count; i++) {
  //     visionObj newObj;
  //     newObj.sig = temp[i].signature;
  //     newObj.x = temp[i].left_coord;
  //     newObj.y = temp[i].top_coord;
  //     newObj.width = temp[i].width;
  //     newObj.height = temp[i].height;
  //     newObj.area = temp[i].width * temp[i].height;
  //     newObj.centerX = temp[i].x_middle_coord;
  //     newObj.centerY = temp[i].y_middle_coord;
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
  //     for (int i = 0; i < readCount; i++)
  //     {
  //       if(temp[i].signature != VISION_OBJECT_ERR_SIG)
  //       {
  //         if(temp[i].signature != wantedSig) { std::cout << "REEEE: " << temp[i].signature << "\n"; }
  //         destContainer.objectArray.at(i + destContainer.currentCount).objSig = temp[i].signature;
  //         destContainer.objectArray.at(i + destContainer.currentCount).objX = temp[i].left_coord;
  //         destContainer.objectArray.at(i + destContainer.currentCount).objY = temp[i].top_coord;
  //         destContainer.objectArray.at(i + destContainer.currentCount).objWidth = temp[i].width;
  //         destContainer.objectArray.at(i + destContainer.currentCount).objHeight = temp[i].height;
  //         destContainer.objectArray.at(i + destContainer.currentCount).objArea = temp[i].height * temp[i].width;
  //         destContainer.objectArray.at(i + destContainer.currentCount).objCenterX = temp[i].x_middle_coord;
  //         destContainer.objectArray.at(i + destContainer.currentCount).objCenterY = temp[i].y_middle_coord;
  //         destContainer.objectArray.at(i + destContainer.currentCount).discardObject = false;
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
  //     for (int i = 0; i < count; i++)
  //     {
  //       destContainer.objectArray.at(i + destContainer.currentCount).objSig = wantedCode.destSig;
  //       destContainer.objectArray.at(i + destContainer.currentCount).objX = temp[i].left_coord;
  //       destContainer.objectArray.at(i + destContainer.currentCount).objY = temp[i].top_coord;
  //       destContainer.objectArray.at(i + destContainer.currentCount).objWidth = temp[i].width;
  //       destContainer.objectArray.at(i + destContainer.currentCount).objHeight = temp[i].height;
  //       destContainer.objectArray.at(i + destContainer.currentCount).objArea = temp[i].height * temp[i].width;
  //       destContainer.objectArray.at(i + destContainer.currentCount).objCenterX = temp[i].x_middle_coord;
  //       destContainer.objectArray.at(i + destContainer.currentCount).objCenterY = temp[i].y_middle_coord;
  //       destContainer.objectArray.at(i + destContainer.currentCount).discardObject = false;
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
