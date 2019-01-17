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
    std::string autoName;
    std::function<void(lib7842::autonSides)> autoFunction;
  };


  class AutonSelector
  {

  private:

    lv_obj_t *m_screenParent = nullptr;
    std::vector<autonPair> m_autonPairs;

  public:

    AutonSelector(lv_obj_t *screenParent, std::initializer_list<autonPair> autonPairs);

  };

}
