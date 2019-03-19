#pragma once
#include "main.h"

class DisplayController
{

public:

  lv_obj_t* tabview = nullptr;
  lv_style_t pageStyle;

  DisplayController(lv_obj_t*);
  ~DisplayController();

  lv_obj_t* newTab(std::string);

};
