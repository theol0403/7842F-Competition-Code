#pragma once
#include "main.h"



namespace lib7842
{

  enum class autonSides
  {
    blue,
    red
  };

  struct autonPair
  {
    std::string autonName;
    std::function<void(lib7842::autonSides)> autonFunc;
  };


  class AutonSelector
  {

  private:

    std::vector<autonPair> m_autonPairs;

    lv_obj_t *m_screenContainer = nullptr;

    int m_currentAutonIndex = 0;
    autonSides m_currentSide = autonSides::red;


  public:

    AutonSelector(lv_obj_t*, std::initializer_list<autonPair>);

    static lv_res_t matrixAction(lv_obj_t*, const char*);
    static lv_res_t sliderAction(lv_obj_t*);

    void run();
    void unlockDriver(std::function<void()>);

  };

}
