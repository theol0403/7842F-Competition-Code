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

    for(int autonNum = 0; autonNum < autonCount; autonNum++)
    {
      buttonNames[autonNum] = &m_autonPairs[autonNum].autonName[0];
    }

    buttonNames[autonCount] = "";


    /*Create a default button matrix*/
    lv_obj_t* buttonMatrix = lv_btnm_create(m_screenContainer, NULL);
    lv_btnm_set_map(buttonMatrix, buttonNames);
    lv_obj_set_size(buttonMatrix, LV_HOR_RES, LV_VER_RES / 2);
    lv_btnm_set_toggle(buttonMatrix, true, currentAutonIndex);
    lv_btnm_set_action(buttonMatrix, matrixAction);
    lv_obj_set_free_ptr(buttonMatrix, this);

  }


  /*Called when a button is released ot long pressed*/
  lv_res_t AutonSelector::matrixAction(lv_obj_t* buttonMatrix, const char *txt)
  {
    AutonSelector* thisSelector = static_cast<AutonSelector*>(lv_obj_get_free_ptr(buttonMatrix));

    for(int autonNum = 0; autonNum < thisSelector->m_autonPairs.size(); autonNum++)
    {
      if(strcmp(txt, &thisSelector->m_autonPairs[autonNum].autonName[0]) == 0)
      {
        std::cout << "Auton: " << autonNum << std::endl;
        thisSelector->currentAutonIndex = autonNum;
      }
    }

    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
  }







}
