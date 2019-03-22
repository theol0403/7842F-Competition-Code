#pragma once
#include "main.h"

class MainDisplay
{

public:

  lv_obj_t* tabview = nullptr;
  lv_style_t pageStyle;

  MainDisplay(lv_obj_t*);
  ~MainDisplay();

  lv_obj_t* newTab(std::string);

};
