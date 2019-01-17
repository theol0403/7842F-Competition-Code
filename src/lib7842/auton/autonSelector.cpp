#include "autonSelector.hpp"

namespace lib7842
{

  AutonSelector::AutonSelector(lv_obj_t *screenParent, std::initializer_list<autonPair> autonPairs)
  :
  m_autonPairs(autonPairs)
  {

    m_screenContainer = lv_obj_create(screenParent, NULL);

    lv_obj_set_size(m_screenContainer, lv_obj_get_width(screenParent), lv_obj_get_height(screenParent));
    lv_obj_align(m_screenContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);

    // Style for background of screen
    lv_style_copy(&m_screenStyle, &lv_style_plain_color);
    m_screenStyle.body.main_color = LV_COLOR_GRAY;
    m_screenStyle.body.grad_color = LV_COLOR_GRAY;
    lv_obj_set_style(m_screenContainer, &m_screenStyle);


    int autonCount = m_autonPairs.size();
    const char** btnm_map = new const char* [autonCount + 1];
    btnm_map[0] = &m_autonPairs[0].autonName[0];

    for(int autonNum = 0; autonNum < autonCount; autonNum++)
    {
      btnm_map[autonNum] = &m_autonPairs[autonNum].autonName[0];
    }

    btnm_map[autonCount] = "";



  }





}
