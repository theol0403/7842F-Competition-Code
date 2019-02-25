#pragma once
#include "main.h"

class ArmController
{

public:

  enum armStates
  {
    off, //motors off
    unfold, //brings it to out position
    out, //holds in out position
    down, //presses down to cap position
    up //brings up to descore position but allows upwards movement
  };

  Motor* arm = nullptr;
  pros::ADIPotentiometer* armSensor = nullptr;
  double foldAngle = 0;
  IterativePosPidController* pid = nullptr;
  pros::Task armTask;

  armStates armState = off;

  ArmController(Motor*, pros::ADIPotentiometer*, double, IterativePosPidController*);

  void setState(armStates);
  double getArmAngle();

  void run();

  static void task(void*);

};
