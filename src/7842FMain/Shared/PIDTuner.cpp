#include "PIDTuner.hpp"

PIDScreenTuner::PIDScreenTuner(int tunerWidth, int tunerHeight, int buttonHeight)
:
m_tunerWidth(tunerWidth), m_tunerHeight(tunerHeight), m_buttonHeight(buttonHeight)
{

  //Screen Container
  //Object
  m_screenContainer = lv_obj_create(lv_scr_act(), NULL);
  lv_obj_set_size(m_screenContainer, m_tunerWidth, m_tunerHeight);
  lv_obj_align(m_screenContainer, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
  //Style
  lv_style_copy(&m_screenContainerStyle, &lv_style_plain_color);
  m_screenContainerStyle.body.main_color = LV_COLOR_GRAY;
  m_screenContainerStyle.body.grad_color = LV_COLOR_GRAY;
  lv_obj_set_style(m_screenContainer, &m_screenContainerStyle);

  //Buttons Container
  //Object
  m_buttonsContainer = lv_obj_create(m_screenContainer, NULL);
  lv_obj_set_size(m_buttonsContainer, m_tunerWidth, m_buttonHeight);
  lv_obj_align(m_buttonsContainer, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  //Style
  lv_style_copy(&m_buttonsContainerStyle, &lv_style_plain_color);
  m_buttonsContainerStyle.body.main_color = LV_COLOR_BLUE;
  m_buttonsContainerStyle.body.grad_color = LV_COLOR_BLUE;
  lv_obj_set_style(m_buttonsContainer, &m_buttonsContainerStyle);
  //Text
  lv_style_copy(&m_buttonTextStyle, &lv_style_plain_color);
  m_buttonTextStyle.text.font = &lv_font_dejavu_20;
  m_buttonTextStyle.text.color = LV_COLOR_BLACK;


  //Info Container
  //Object
  m_infoContainer = lv_obj_create(m_screenContainer, NULL);
  lv_obj_set_size(m_infoContainer, m_tunerWidth, m_tunerHeight - m_buttonHeight);
  lv_obj_align(m_infoContainer, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
  //Style
  lv_style_copy(&m_infoContainerStyle, &lv_style_plain_color);
  m_infoContainerStyle.body.main_color = LV_COLOR_ORANGE;
  m_infoContainerStyle.body.grad_color = LV_COLOR_ORANGE;
  lv_obj_set_style(m_infoContainer, &m_infoContainerStyle);



  //Gauge
  lv_style_copy(&m_gaugeStyle, &lv_style_pretty_color);
  m_gaugeStyle.body.main_color = LV_COLOR_HEX3(0x666);     /*Line color at the beginning*/
  m_gaugeStyle.body.grad_color =  LV_COLOR_HEX3(0x666);    /*Line color at the end*/
  m_gaugeStyle.body.padding.hor = 10;                      /*Scale line length*/
  m_gaugeStyle.body.padding.inner = 8 ;                    /*Scale label padding*/
  m_gaugeStyle.body.border.color = LV_COLOR_HEX3(0x333);   /*Needle middle circle color*/
  m_gaugeStyle.line.width = 3;
  m_gaugeStyle.text.color = LV_COLOR_HEX3(0x333);
  m_gaugeStyle.line.color = LV_COLOR_RED;                  /*Line color after the critical value*/
  //Needle Color
  m_needleColors[0] = LV_COLOR_BLUE;
  m_needleColors[1] = LV_COLOR_RED;
  m_needleColors[3] = LV_COLOR_PURPLE;
}

PIDScreenTuner::~PIDScreenTuner()
{

}



void PIDScreenTuner::initButton(int xPos, double* variablePtr, const char* variableName, int charLength, buttonType_t buttonType, int incrementAmount)
{
  tunerButtons_t* tunerButtons = new tunerButtons_t;
  tunerButtons->variablePtr = variablePtr;

  tunerButtons->variableName = new char[5];
  sprintf(tunerButtons->variableName, variableName);
  tunerButtons->charLength = charLength + (strlen(variableName) + 1);
  tunerButtons->labelContent = new char[10];

  tunerButtons->buttonType = buttonType;
  tunerButtons->multiplierPtr = &m_buttonMultiplier;
  tunerButtons->incrementAmount = incrementAmount;

  //Minus Button
  tunerButtons->minusButton = lv_btn_create(m_buttonsContainer, NULL);
  lv_obj_set_size(tunerButtons->minusButton, m_buttonHeight/1.3, m_buttonHeight/2);
  lv_obj_align(tunerButtons->minusButton, m_buttonsContainer, LV_ALIGN_IN_BOTTOM_LEFT, xPos, -m_buttonHeight/2);
  //Label
  lv_obj_t * minusLabel = lv_label_create(tunerButtons->minusButton, NULL);
  lv_obj_align(tunerButtons->minusButton, minusLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_label_set_text(minusLabel, "-");

  //Plus Button
  tunerButtons->plusButton = lv_btn_create(m_buttonsContainer, NULL);
  lv_obj_set_size(tunerButtons->plusButton, m_buttonHeight/1.3, m_buttonHeight/2);
  lv_obj_align(tunerButtons->plusButton, m_buttonsContainer, LV_ALIGN_IN_BOTTOM_LEFT, xPos, -m_buttonHeight*2/2);
  //Label
  lv_obj_t * plusLabel = lv_label_create(tunerButtons->plusButton, NULL);
  lv_obj_align(tunerButtons->plusButton, plusLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_label_set_text(plusLabel, "+");

  //Assign ID, assign direction, assign function
  lv_obj_set_free_ptr(tunerButtons->minusButton, tunerButtons);
  lv_obj_set_free_ptr(tunerButtons->plusButton, tunerButtons);
  lv_obj_set_free_num(tunerButtons->minusButton, buttonMinus);
  lv_obj_set_free_num(tunerButtons->plusButton, buttonPlus);
  lv_btn_set_action(tunerButtons->minusButton, LV_BTN_ACTION_CLICK, buttonAction);
  lv_btn_set_action(tunerButtons->plusButton, LV_BTN_ACTION_CLICK, buttonAction);


  //Button Labels
  tunerButtons->buttonLabel = lv_label_create(m_buttonsContainer, NULL);
  lv_obj_set_style(tunerButtons->buttonLabel, &m_buttonTextStyle);
  lv_label_set_long_mode(tunerButtons->buttonLabel, LV_LABEL_LONG_BREAK);
  lv_obj_set_size(tunerButtons->buttonLabel, m_buttonHeight, m_buttonHeight);
  lv_obj_align(tunerButtons->buttonLabel, m_buttonsContainer, LV_ALIGN_IN_BOTTOM_LEFT, xPos, -m_buttonHeight/2);

  //Assign Label Text
  sprintf(tunerButtons->labelContent, "%s\n%f", tunerButtons->variableName, *tunerButtons->variablePtr);
  tunerButtons->labelContent[tunerButtons->charLength] = '\0';
  lv_label_set_text(tunerButtons->buttonLabel, tunerButtons->labelContent);

}


lv_res_t PIDScreenTuner::buttonAction(lv_obj_t* btn)
{
  tunerButtons_t* tunerButtons = static_cast<tunerButtons_t*>(lv_obj_get_free_ptr(btn));
  buttonDirection_t buttonDirection = static_cast<buttonDirection_t>(lv_obj_get_free_num(btn));

  switch(tunerButtons->buttonType)
  {
    case buttonAdd:
    {
      if(buttonDirection == buttonMinus)
      {
        *tunerButtons->variablePtr -= *tunerButtons->multiplierPtr;
      }
      else if(buttonDirection == buttonPlus)
      {
        *tunerButtons->variablePtr += *tunerButtons->multiplierPtr;
      }
      //Stops button from going smaller than 0
      if(*tunerButtons->variablePtr <= 0) *tunerButtons->variablePtr = 0.00000;
      break;
    }

    case buttonMultiply:
    {
      if(buttonDirection == buttonMinus)
      {
        *tunerButtons->variablePtr /= tunerButtons->incrementAmount;
      }
      else if(buttonDirection == buttonPlus)
      {
        *tunerButtons->variablePtr *= tunerButtons->incrementAmount;
      }
      break;
    }

    case buttonIncrement:
    {
      if(buttonDirection == buttonMinus)
      {
        *tunerButtons->variablePtr -= tunerButtons->incrementAmount;
      }
      else if(buttonDirection == buttonPlus)
      {
        *tunerButtons->variablePtr += tunerButtons->incrementAmount;
      }
      break;
    }

    default:
    {

    }
  }

  //Assign Label Text
  sprintf(tunerButtons->labelContent, "%s\n%f", tunerButtons->variableName, *tunerButtons->variablePtr);
  tunerButtons->labelContent[tunerButtons->charLength] = '\0';
  lv_label_set_text(tunerButtons->buttonLabel, tunerButtons->labelContent);

  return LV_RES_OK;
}



lv_obj_t* PIDScreenTuner::initGauge(int xPos, const char* variableName, int needleCount, int minReading, int maxReading)
{
/*Create a gauge*/
lv_obj_t* newGauge = lv_gauge_create(m_infoContainer, NULL);
lv_obj_set_size(newGauge, m_tunerHeight - m_buttonHeight, m_tunerHeight - m_buttonHeight);
lv_gauge_set_range(newGauge, minReading, maxReading);
lv_gauge_set_critical_value(newGauge, maxReading);
lv_gauge_set_needle_count(newGauge, needleCount, m_needleColors);
lv_gauge_set_style(newGauge, &m_gaugeStyle);
lv_obj_align(newGauge, m_infoContainer, LV_ALIGN_IN_LEFT_MID, xPos, 25);

return newGauge;

}





lv_obj_t* PIDScreenTuner::initMeter(int xPos, const char* variableName, int minReading, int maxReading)
{
/*Create a gauge*/
lv_obj_t* newMeter = lv_gauge_create(m_infoContainer, NULL);
lv_obj_set_size(newMeter, m_tunerHeight - m_buttonHeight, m_tunerHeight - m_buttonHeight);
lv_lmeter_set_range(newMeter, minReading, maxReading);
lv_lmeter_set_style(newMeter, &m_gaugeStyle);
lv_obj_align(newMeter, m_infoContainer, LV_ALIGN_IN_LEFT_MID, xPos, 25);

return newMeter;

}
