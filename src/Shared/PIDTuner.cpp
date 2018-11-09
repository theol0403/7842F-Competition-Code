#include "main.h"
#include "Include/Shared/PIDTuner.hpp"



    extern float countMultiplier;

PIDScreenTuner::PIDScreenTuner(pidTune_t* pidTune, int containerWidth, int containerHeight)
{

  m_tunerContainer = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(m_tunerContainer, containerWidth, containerHeight);
  lv_obj_align(m_tunerContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
  // Style for background of screen
  lv_style_copy(&m_tunerContainerStyle, &lv_style_plain_color);
  m_tunerContainerStyle.body.main_color = LV_COLOR_GRAY;
  m_tunerContainerStyle.body.grad_color = LV_COLOR_GRAY;
  lv_obj_set_style(m_tunerContainer, &m_tunerContainerStyle);

  m_buttonContainer = lv_obj_create(m_tunerContainer, NULL);
  lv_obj_set_size(m_buttonContainer, containerWidth, 100);
  lv_obj_align(m_buttonContainer, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  // Style for buttons area
  lv_style_copy(&m_buttonContainerStyle, &lv_style_plain_color);
  m_buttonContainerStyle.body.main_color = LV_COLOR_BLUE;
  m_buttonContainerStyle.body.grad_color = LV_COLOR_BLUE;
  lv_obj_set_style(m_buttonContainer, &m_buttonContainerStyle);



}



PIDScreenTuner::~PIDScreenTuner()
{

}


void PIDScreenTuner::initButton(tunerButtons_t* tunerButtons, int xPos, float* variablePtr, const char* variableName, bool multiplier)
{
  tunerButtons->minusButton = lv_btn_create(m_buttonContainer, NULL);
  lv_obj_set_size(tunerButtons->minusButton, 50, 50);
  //lv_obj_set_pos(tunerButtons->minusButton, xPos, yPos);
  lv_obj_align(tunerButtons->minusButton, m_buttonContainer, LV_ALIGN_OUT_BOTTOM_LEFT, xPos, -75);
  /*Add a label to the button*/
  lv_obj_t * minusLabel = lv_label_create(tunerButtons->minusButton, NULL);
  lv_obj_align(tunerButtons->minusButton, minusLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_label_set_text(minusLabel, "-");


  tunerButtons->plusButton = lv_btn_create(m_buttonContainer, NULL);
  lv_obj_set_size(tunerButtons->plusButton, 50, 50);
  //lv_obj_set_pos(tunerButtons->plusButton, xPos + 40, yPos);
  lv_obj_align(tunerButtons->plusButton, m_buttonContainer, LV_ALIGN_OUT_BOTTOM_LEFT, xPos+50, -75);
  /*Add a label to the button*/
  lv_obj_t * plusLabel = lv_label_create(tunerButtons->plusButton, NULL);
  lv_obj_align(tunerButtons->plusButton, plusLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_label_set_text(plusLabel, "+");

  lv_obj_set_free_ptr(tunerButtons->minusButton, tunerButtons);
  lv_btn_set_action(tunerButtons->minusButton, LV_BTN_ACTION_CLICK, tuneClickAction);

  lv_obj_set_free_ptr(tunerButtons->plusButton, tunerButtons);
  lv_btn_set_action(tunerButtons->plusButton, LV_BTN_ACTION_CLICK, tuneClickAction);

  if(!multiplier)
  {
      lv_obj_set_free_num(tunerButtons->minusButton, 1);
      lv_obj_set_free_num(tunerButtons->plusButton, 2);
  }
  else
  {
    lv_obj_set_free_num(tunerButtons->minusButton, 3);
    lv_obj_set_free_num(tunerButtons->plusButton, 4);
  }


tunerButtons->textLabel = lv_label_create(m_buttonContainer, NULL);
  lv_label_set_text(tunerButtons->textLabel, variableName);
  lv_obj_align(tunerButtons->textLabel, m_buttonContainer, LV_ALIGN_OUT_BOTTOM_LEFT, xPos, -90);
  tunerButtons->variableLabel = lv_label_create(m_buttonContainer, NULL);
  char* variableContent = new char[20];
  sprintf(variableContent, "%f", *variablePtr);
  lv_label_set_text(tunerButtons->variableLabel, variableContent);
  delete[] variableContent;
  lv_obj_align(tunerButtons->variableLabel, m_buttonContainer, LV_ALIGN_OUT_BOTTOM_LEFT, xPos, -70);

  tunerButtons->variablePtr = variablePtr;
}



lv_res_t PIDScreenTuner::tuneClickAction(lv_obj_t* btn)
{

  incrementVariable(lv_obj_get_free_num(btn), static_cast <tunerButtons_t*>(lv_obj_get_free_ptr(btn)));
  pros::delay(50);       /*Just to let the system breath*/
  return LV_RES_OK;
}


void PIDScreenTuner::incrementVariable(int actionType, tunerButtons_t* tunerButtons)
{
  if(actionType == 1)
  {
    *tunerButtons->variablePtr = *tunerButtons->variablePtr - countMultiplier;
    if(*tunerButtons->variablePtr <= 0) *tunerButtons->variablePtr = 0.001;
  }
  else if(actionType == 2)
  {
    *tunerButtons->variablePtr = *tunerButtons->variablePtr + countMultiplier;
  }
  else if(actionType == 3)
  {
    *tunerButtons->variablePtr /= 10;
  }
  else if(actionType == 4)
  {
    *tunerButtons->variablePtr *= 10;
  }


  char* variableContent = new char[20];
  sprintf(variableContent, "%f", *tunerButtons->variablePtr);
  lv_label_set_text(tunerButtons->variableLabel, variableContent);
  delete[] variableContent;
}
