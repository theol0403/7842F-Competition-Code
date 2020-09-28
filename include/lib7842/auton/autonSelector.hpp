#pragma once
#include "lib7842/odometry/odomMath.hpp"
#include "main.h"

namespace lib7842 {
struct autonPair {
  std::string autonName;
  std::function<void(void*)> autonFunc;
};

class AutonSelector {

private:
  std::vector<autonPair> m_autonPairs;

  lv_obj_t* m_screenContainer = nullptr;

public:
  int m_currentAutonIndex = 0;
  autonSides m_currentSide = autonSides::red;

  AutonSelector(lv_obj_t*, std::initializer_list<autonPair>);
  AutonSelector(lv_obj_t*, lv_color_t, std::initializer_list<autonPair>);

  static lv_res_t matrixAction(lv_obj_t*, const char*);
  static lv_res_t sliderAction(lv_obj_t*);

  autonPair getSelectedAuton();
  autonSides getSelectedSide();
};

} // namespace lib7842
