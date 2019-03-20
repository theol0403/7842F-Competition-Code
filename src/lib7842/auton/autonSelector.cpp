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

    lv_style_t* screenStyle = new lv_style_t;
    lv_style_copy(screenStyle, &lv_style_plain_color);
    lv_color_t mainColor = LV_COLOR_HEX(0xFF7F00);
    screenStyle->body.main_color = mainColor;
    screenStyle->body.grad_color = mainColor;
    lv_obj_set_style(m_screenContainer, screenStyle);


    int autonCount = m_autonPairs.size();
    const char** buttonNames = new const char* [autonCount + 1];

    for(int autonNum = 0; autonNum < autonCount; autonNum++)
    {
      buttonNames[autonNum] = &m_autonPairs[autonNum].autonName[0];
    }

    buttonNames[autonCount] = "";


    lv_obj_t* buttonMatrix = lv_btnm_create(m_screenContainer, NULL);
    lv_btnm_set_map(buttonMatrix, buttonNames);
    lv_obj_set_size(buttonMatrix, lv_obj_get_width(screenParent), lv_obj_get_height(screenParent) / 2);
    lv_btnm_set_toggle(buttonMatrix, true, m_currentAutonIndex);
    lv_btnm_set_action(buttonMatrix, matrixAction);
    lv_obj_set_free_ptr(buttonMatrix, this);


    /*Create a new style for the button matrix back ground*/
    lv_style_t* style_bg = new lv_style_t;
    lv_style_copy(style_bg, &lv_style_plain);
    style_bg->body.main_color = mainColor;
    style_bg->body.grad_color = mainColor;
    style_bg->body.border.color = LV_COLOR_WHITE;
    style_bg->body.border.width = 3;
    style_bg->body.radius = 0;
    lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BG, style_bg);

    lv_style_t* style_rel = new lv_style_t;
    lv_style_copy(style_rel, &lv_style_btn_tgl_rel);
    style_rel->body.main_color = mainColor;
    style_rel->body.grad_color = mainColor;
    style_rel->body.border.color = LV_COLOR_WHITE;
    style_rel->body.border.width = 3;
    style_rel->body.border.opa = LV_OPA_100;
    style_rel->body.radius = 10;
    style_rel->text.color = LV_COLOR_WHITE;
    lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_TGL_REL, style_rel);

    lv_style_t* style_pr = new lv_style_t;
    lv_style_copy(style_pr, style_rel);
    style_pr->body.main_color = LV_COLOR_WHITE;
    style_pr->body.grad_color = LV_COLOR_WHITE;
    style_pr->text.color = mainColor;
    lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_TGL_PR, style_pr);
    lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_PR, style_pr);

    lv_style_t* style_ina = new lv_style_t;
    lv_style_copy(style_ina, &lv_style_btn_ina);
    style_ina->body.main_color = mainColor;
    style_ina->body.grad_color = mainColor;
    style_ina->body.border.width = 0;
    style_ina->text.color = LV_COLOR_WHITE;
    lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_INA, style_ina);
    lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BTN_REL, style_ina);

    //SWITCH

    lv_obj_t *colorSwitch = lv_sw_create(m_screenContainer, NULL);
    lv_obj_set_size(colorSwitch, lv_obj_get_width(screenParent)/3, lv_obj_get_height(screenParent)/4);
    lv_obj_align(colorSwitch, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -lv_obj_get_height(screenParent)/8);
    //  lv_sw_set_anim_time(colorSwitch, 200);
    lv_sw_set_action(colorSwitch, sliderAction);
    lv_obj_set_free_ptr(colorSwitch, this);

    lv_style_t* style_sw_bg = new lv_style_t;
    lv_style_copy(style_sw_bg, style_bg);
    style_sw_bg->body.radius = 20;
    lv_sw_set_style(colorSwitch, LV_SW_STYLE_BG, style_sw_bg);

    lv_style_t* indic_style = new lv_style_t;
    lv_style_copy(indic_style, &lv_style_pretty_color);
    indic_style->body.empty = 1;
    indic_style->body.border.width = 0;
    lv_sw_set_style(colorSwitch, LV_SW_STYLE_INDIC, indic_style);

    lv_style_t* knob_off_style = new lv_style_t;
    lv_style_copy(knob_off_style, &lv_style_pretty);
    knob_off_style->body.main_color = LV_COLOR_RED;
    knob_off_style->body.grad_color = LV_COLOR_RED;
    knob_off_style->body.opa = LV_OPA_100;
    knob_off_style->body.radius = LV_RADIUS_CIRCLE;
    knob_off_style->body.shadow.width = 0;
    knob_off_style->body.border.width = 3;
    knob_off_style->body.border.color = LV_COLOR_WHITE;
    knob_off_style->body.border.opa = LV_OPA_100;
    lv_sw_set_style(colorSwitch, LV_SW_STYLE_KNOB_OFF, knob_off_style);

    lv_style_t* knob_on_style = new lv_style_t;
    lv_style_copy(knob_on_style, knob_off_style);
    knob_on_style->body.main_color = LV_COLOR_BLUE;
    knob_on_style->body.grad_color = LV_COLOR_BLUE;
    lv_sw_set_style(colorSwitch, LV_SW_STYLE_KNOB_ON, knob_on_style);

  }


  lv_res_t AutonSelector::matrixAction(lv_obj_t* buttonMatrix, const char *txt)
  {
    AutonSelector* that = static_cast<AutonSelector*>(lv_obj_get_free_ptr(buttonMatrix));

    for(int autonNum = 0; autonNum < that->m_autonPairs.size(); autonNum++)
    {
      if(strcmp(txt, &that->m_autonPairs[autonNum].autonName[0]) == 0)
      {
        std::cout << "Auton: " << autonNum << std::endl;
        that->m_currentAutonIndex = autonNum;
      }
    }

    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
  }


  lv_res_t AutonSelector::sliderAction(lv_obj_t* slider)
  {
    AutonSelector* that = static_cast<AutonSelector*>(lv_obj_get_free_ptr(slider));

    that->m_currentSide = lv_sw_get_state(slider) ? autonSides::blue : autonSides::red;

    std::cout << "Switch: " << ((that->m_currentSide == autonSides::red) ? "Red" : "Blue") << std::endl;

    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
  }


  autonPair AutonSelector::getSelectedAuton()
  {
    return m_autonPairs[m_currentAutonIndex];
  }

  autonSides AutonSelector::getSelectedSide()
  {
    return m_currentSide;
  }

}
