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

    lv_style_copy(&m_screenStyle, &lv_style_plain_color);
    lv_color_t mainColor = LV_COLOR_HEX(0xFF7F00);
    m_screenStyle.body.main_color = mainColor;
    m_screenStyle.body.grad_color = mainColor;
    lv_obj_set_style(m_screenContainer, &m_screenStyle);


    int autonCount = m_autonPairs.size();
    const char** buttonNames = new const char* [autonCount + 1];

    for(int autonNum = 0; autonNum < autonCount; autonNum++)
    {
      buttonNames[autonNum] = &m_autonPairs[autonNum].autonName[0];
    }

    buttonNames[autonCount] = "";


    lv_obj_t* buttonMatrix = lv_btnm_create(m_screenContainer, NULL);
    lv_btnm_set_map(buttonMatrix, buttonNames);
    lv_obj_set_size(buttonMatrix, lv_obj_get_width(screenParent), lv_obj_get_height(screenParent) / 3);
    lv_btnm_set_toggle(buttonMatrix, true, currentAutonIndex);
    lv_btnm_set_action(buttonMatrix, matrixAction);
    lv_obj_set_free_ptr(buttonMatrix, this);


    /*Create a new style for the button matrix back ground*/
    lv_style_t* style_bg = new lv_style_t;
    lv_style_copy(style_bg, &lv_style_plain);
    style_bg->body.main_color = mainColor;
    style_bg->body.grad_color = mainColor;
    style_bg->body.border.color = LV_COLOR_WHITE;
    style_bg->body.border.width = 3;
    style_bg->body.radius = 25;
    lv_btnm_set_style(buttonMatrix, LV_BTNM_STYLE_BG, style_bg);

    lv_style_t* style_rel = new lv_style_t;
    lv_style_copy(style_rel, &lv_style_btn_tgl_rel);
    style_rel->body.main_color = mainColor;
    style_rel->body.grad_color = mainColor;
    style_rel->body.border.color = LV_COLOR_WHITE;
    style_rel->body.border.width = 3;
    style_rel->body.border.opa = LV_OPA_100;
    style_rel->body.radius = 15;
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

    // LV_BTNM_STYLE_BG Background style. Uses all style.body properties including padding Default: _lv_style_pretty_

    // LV_BTNM_STYLE_BTN_TGL_REL style of the toggled released buttons. Default: _lv_style_btn_tgl_rel_
    // LV_BTNM_STYLE_BTN_TGL_PR style of the toggled pressed buttons. Default: _lv_style_btn_tgl_pr_
    // LV_BTNM_STYLE_BTN_INA style of the inactive buttons. Default: _lv_style_btn_ina_


    // /*Create 2 button styles*/
    // lv_style_t style_btn_rel;
    // lv_style_t style_btn_pr;
    // lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
    // style_btn_rel.body.main_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);
    // style_btn_rel.body.grad_color = LV_COLOR_BLACK;
    // style_btn_rel.body.border.color = LV_COLOR_SILVER;
    // style_btn_rel.body.border.width = 1;
    // style_btn_rel.body.border.opa = LV_OPA_50;
    // style_btn_rel.body.radius = 0;
    //
    // lv_style_copy(&style_btn_pr, &style_btn_rel);
    // style_btn_pr.body.main_color = LV_COLOR_MAKE(0x55, 0x96, 0xd8);
    // style_btn_pr.body.grad_color = LV_COLOR_MAKE(0x37, 0x62, 0x90);
    // style_btn_pr.text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);

    /*Create a second button matrix with the new styles*/
    // lv_obj_t * btnm2 = lv_btnm_create(lv_scr_act(), btnm1);
    // lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BG, &style_bg);
    // lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BTN_REL, &style_btn_rel);
    // lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BTN_PR, &style_btn_pr);
    // lv_obj_align(btnm2, btnm1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

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
