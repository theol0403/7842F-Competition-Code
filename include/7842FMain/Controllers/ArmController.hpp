#pragma once
#include "main.h"

class ArmController
{

public:

  enum armStates
  {
    off, //motors off
    down,
    aboveWall,
    descore
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
