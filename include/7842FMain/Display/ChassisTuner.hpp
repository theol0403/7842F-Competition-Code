#pragma once
#include "main.h"
#include <map>

class ChassisTuner
{

public:

  lv_obj_t* container = nullptr;

  ChassisTuner(lv_obj_t*);
  ChassisTuner(lv_obj_t*, lv_color_t);
  ~ChassisTuner();

  void buildButton();

  static lv_res_t btnAction(lv_obj_t*, const char*);

};
