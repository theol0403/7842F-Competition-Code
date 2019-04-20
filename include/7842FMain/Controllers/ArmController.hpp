#pragma once
#include "main.h"

class ArmController
{

public:

  enum armStates
  {
    off, //motors off
    hold,
    holdAtPos,
    down,
    carry,
    balance,
    descore,
    up,
    upSlow
  };

  Motor* arm = nullptr;
  double startAngle = 0;
  IterativePosPIDController* pid = nullptr;

  pros::Task task;

  armStates armState = off;

  ArmController(Motor*, IterativePosPIDController*);

  void setState(armStates);
  armStates getState();
  double getArmAngle();

  void run();
  static void taskFnc(void*);

};
