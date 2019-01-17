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
    const char** buttonNames = new const char* [autonCount + 1];
    buttonNames[0] = &m_autonPairs[0].autonName[0];

    for(int autonNum = 0; autonNum < autonCount; autonNum++)
    {
      buttonNames[autonNum] = &m_autonPairs[autonNum].autonName[0];
    }

    buttonNames[autonCount] = "";


    /*Create a default button matrix*/
    lv_obj_t* buttonMatrix = lv_btnm_create(m_screenContainer, NULL);
    lv_btnm_set_map(buttonMatrix, buttonNames);
    lv_obj_set_size(buttonMatrix, LV_HOR_RES, LV_VER_RES / 2);
    lv_btnm_set_toggle(buttonMatrix, true, 0);
    lv_btnm_set_action(buttonMatrix, autonAction);

  }


  /*Called when a button is released ot long pressed*/
lv_res_t AutonSelector::autonAction(lv_obj_t* btnm, const char *txt)
{
  pros::delay(200);
    printf("Button Number: %d released\n", lv_btnm_get_toggled(btnm));

    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}





}
