#include "visionReader.hpp"

namespace lib7842
{

  VisionReader::VisionReader(pros::Vision* ivision) :
  vision(ivision), maxCount(20) {
    temp.resize(maxCount);
  }


  void VisionReader::getAll()
  {
    int count = vision->read_by_size(0, maxCount, temp.data());
    if(count > maxCount) count = 0; //If there are no objects pros returns a huge number

    for (int i = 0; i < count; i++) addObj(temp.at(i));
  }

  void VisionReader::getSig(int sig) {
    int count = vision->read_by_sig(0, sig, maxCount, temp.data());
    if(count > maxCount) count = 0; //If there are no objects pros returns a huge number

    for (int i = 0; i < count; i++) {
      assert(temp.at(i).signature == sig);
      addObj(temp.at(i));
    }
  }

  void VisionReader::getSig(std::initializer_list<int> sigs)
  {
    for(const int &sig : sigs) getSig(sig);
  }


  // void VisionReader::getCodeObjects(lib7842::ObjContainer &destContainer, std::vector<codeSig_t> wantedCodes)
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
  // codeSig_t VisionReader::createCodeSig(int sig_id1, int sig_id2, int sig_id3, int sig_id4, int sig_id5)
  // {
  //   codeSig_t tempCode;
  //   tempCode.destSig = sig_id1 + (NUM_SIGNATURES/2);
  //   tempCode.colorCode = m_thisVision->create_color_code(sig_id1, sig_id2, sig_id3, sig_id4, sig_id5);
  //   return tempCode;
  // }


}
