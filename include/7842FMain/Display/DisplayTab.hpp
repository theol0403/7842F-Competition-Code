#pragma once
#include "main.h"

class DisplayTab
{

public:

  lv_obj_t* tabview = nullptr;
  lv_style_t pageStyle;

  DisplayTab(lv_obj_t*);
  ~DisplayTab();

  lv_obj_t* newTab(std::string);

};
