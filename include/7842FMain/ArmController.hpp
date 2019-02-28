#pragma once
#include "main.h"

class ArmController
{

public:

  enum armStates
  {
    off, //motors off
    down, //presses down to cap position
    back //brings up to descore position but allows upwards movement
  };

  Motor* arm = nullptr;
  double startAngle = 0;
  IterativePosPIDController* pid = nullptr;
  pros::Task armTask;

  armStates armState = off;

  ArmController(Motor*, IterativePosPIDController*);

  void setState(armStates);
  armStates getState();
  double getArmAngle();

  void run();

  static void task(void*);

};
