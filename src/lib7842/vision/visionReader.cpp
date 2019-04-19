#include "visionReader.hpp"
#include <cassert>

namespace lib7842
{

  VisionReader::VisionReader(pros::Vision* ivision) :
  vision(ivision), maxCount(20) {
    temp.resize(maxCount);
  }


  VisionReader &VisionReader::getAll() {
    int count = vision->read_by_size(0, maxCount, temp.data());
    if(count > maxCount) count = 0; //If there are no objects pros returns a huge number
    //add each object to the master array
    for (int i = 0; i < count; i++) {
      addObj(temp.at(i));
    }
    checkErrSig();
    return *this;
  }

  VisionReader &VisionReader::getSig(int sig) {
    int count = vision->read_by_sig(0, sig, maxCount, temp.data());
    if(count > maxCount) count = 0; //If there are no objects pros returns a huge number
    //add each object to the master array
    for (int i = 0; i < count; i++) {
      assert(temp.at(i).signature == sig);
      addObj(temp.at(i));
    }
    checkErrSig();
    return *this;
  }

  VisionReader &VisionReader::getSig(std::initializer_list<int> sigs) {
    for(const int &sig : sigs) getSig(sig);
    return *this;
  }

  uint16_t VisionReader::createCode(int first, int second) {
    return vision->create_color_code(first, second);
  }

}
