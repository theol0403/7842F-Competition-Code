#pragma once
#include "main.h"


namespace lib7842
{

  class FlywheelController
  {

  public:

    AbstractMotor* flywheel = nullptr;
    lib7842::velPID* pid = nullptr;
    lib7842::emaFilter rpmFilter;
    double powerSlew = 1;

    pros::Task flywheelTask;

    double targetRPM = 0;
    double currentRPM = 0;

    double lastPower = 0;
    double finalPower = 0;

    FlywheelController(AbstractMotor*, lib7842::velPID*, double);

    void setRPM(double);
    void disable();
    void enable();

    static void run(void*);

  };
}
