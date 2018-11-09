#pragma once
#include "main.h"

struct pidTune_t
{
  double kP = 0;
  double kI = 0;
  double kD = 0;
  double kF = 0;
  double readingEma = 1;
  double dirivativeEma = 1;
};


struct tunerButtons_t
{
  lv_obj_t* plusButton;
  lv_obj_t* minusButton;
  lv_obj_t* textLabel;
  lv_obj_t* variableLabel;
  char* variableName;
  double* variablePtr;
};


class PIDScreenTuner
{
private:


  _lv_obj_t* m_tunerContainer = nullptr;
  lv_style_t m_tunerContainerStyle;

  _lv_obj_t* m_buttonContainer = nullptr;
  lv_style_t m_buttonContainerStyle;

public:

  PIDScreenTuner(pidTune_t*, int, int);
  ~PIDScreenTuner();

  void initButton(tunerButtons_t*, int, double*, const char*, bool = false);

  static lv_res_t tuneClickAction(lv_obj_t*);
  static void incrementVariable(int, tunerButtons_t*);




};
