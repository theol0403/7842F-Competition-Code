#include "DisplayController.hpp"

DisplayController::DisplayController(lv_obj_t* parent) :
tabview(lv_tabview_create(parent, NULL))
{

}


lv_obj_t* DisplayController::newTab(std::string name) {
  lv_obj_t* tab = lv_tabview_add_tab(tabview, name.c_str());
  lv_obj_align(tab, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
  return tab;
}
