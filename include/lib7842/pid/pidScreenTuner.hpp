#pragma once
#include "main.h"

namespace lib7842
{

  class PIDScreenTuner
  {
  public:

    struct pidTune_t
    {
      double kP = 0;
      double kI = 0;
      double kD = 0;
      double kF = 0;
      double readingEma = 1;
      double derivativeEma = 1;
    };


    enum buttonType_t
    {
      buttonAdd,
      buttonMultiply,
      buttonIncrement
    };

    enum buttonDirection_t
    {
      buttonMinus,
      buttonPlus
    };


    struct tunerButtons_t
    {

      double* variablePtr = nullptr;
      char* variableName = nullptr;
      int charLength = 0;
      char* labelContent = nullptr;

      buttonType_t buttonType = buttonAdd;
      double* multiplierPtr = nullptr;
      int incrementAmount = 1;

      lv_obj_t* minusButton = nullptr;
      lv_obj_t* plusButton = nullptr;

      lv_obj_t* buttonLabel = nullptr;

    };

    lv_obj_t* m_screenContainer = nullptr;
    lv_style_t m_screenContainerStyle;

    const int m_tunerWidth;
    const int m_tunerHeight;

    const int m_buttonHeight;

    lv_obj_t* m_buttonsContainer = nullptr;
    lv_style_t m_buttonsContainerStyle;
    lv_style_t m_buttonTextStyle;


    lv_obj_t* m_infoContainer = nullptr;
    lv_style_t m_infoContainerStyle;


    lv_style_t m_gaugeStyle;
    lv_color_t m_needleColors[5];


    PIDScreenTuner(int, int, int);
    ~PIDScreenTuner();

    double m_buttonMultiplier = 1;
    void initButton(int, double*, const char*, int, buttonType_t = buttonAdd, int = 1);
    static lv_res_t buttonAction(lv_obj_t*);

    lv_obj_t* initGauge(int, const char*, int, int, int);
    lv_obj_t* initMeter(int, const char*, int, int);


  };

}
