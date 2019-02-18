#pragma once
#include "main.h"


namespace lib7842
{

  class FlywheelController
  {

  public:

    AbstractMotor* flywheel = nullptr;
    pros::Task flywheelTask;

    double targetRPM = 0;

    FlywheelController(AbstractMotor*);

    void setRPM(double);
    void disable();
    void enable();

    static void run(void*);

  };
}
