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
    std::function<void(lib7842::autonSides)> autoFunction;
  };


  class AutonSelector
  {

  private:

    std::vector<autonPair> m_autonPairs;

    lv_obj_t *m_screenContainer = nullptr;
    lv_style_t m_screenStyle;

    int currentAutonIndex = 0;


  public:

    AutonSelector(lv_obj_t*, std::initializer_list<autonPair>);

    static lv_res_t matrixAction(lv_obj_t*, const char*);

  };

}
