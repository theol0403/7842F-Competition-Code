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

    std::vector<autonPair> m_autonPairs;

    lv_obj_t *m_screenContainer = nullptr;
    lv_style_t m_screenStyle;


  public:

    AutonSelector(lv_obj_t *screenParent, std::initializer_list<autonPair> autonPairs);

  };

}
