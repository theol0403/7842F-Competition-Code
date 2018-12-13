#pragma once
#include "objectTracking.hpp"

namespace lib7842
{
  struct codeSig_t
  {
    int destSig = VISION_OBJECT_ERR_SIG;
    pros::vision_color_code_t colorCode;
  };

  class ObjectReading
  {

  private:

    pros::Vision* m_thisVision = nullptr;

  public:


    ObjectReading(pros::Vision&);
    ~ObjectReading();

    void getAllObjects(lib7842::ObjectContainer&);
    void getSigObjects(lib7842::ObjectContainer&, std::vector<int>);
    void getCodeObjects(lib7842::ObjectContainer&, std::vector<codeSig_t>);

    codeSig_t createCodeSig(int, int, int = 0, int = 0, int = 0);


  };
}
