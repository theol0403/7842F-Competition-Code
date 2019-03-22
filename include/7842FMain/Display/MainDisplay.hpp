#pragma once
#include "main.h"

class MainDisplay
{

public:

  lv_obj_t* tabview = nullptr;
  lv_color_t mainColor;
  lv_style_t pageStyle;

  MainDisplay(lv_obj_t*, lv_color_t);
  ~MainDisplay();

  lv_obj_t* newTab(std::string);
  lv_obj_t* getParent();
  void splashScreen(const lv_img_t*, int);

};
