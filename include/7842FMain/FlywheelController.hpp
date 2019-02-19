#pragma once
#include "main.h"
#include "lib7842/pid/pidVelSystem.hpp"

class FlywheelController
{

public:

  AbstractMotor* flywheel = nullptr;
  lib7842::velPID* pid = nullptr;
  lib7842::emaFilter rpmFilter;
  double flywheelRatio = 1;
  double slewRate = 1;

  pros::Task flywheelTask;

  double targetRPM = 0;
  double currentRPM = 0;

  double lastPower = 0;
  double finalPower = 0;

  bool disabled = false;

  FlywheelController(AbstractMotor*, lib7842::velPID*, double, double, double);

  void setRPM(double);
  void disable();
  void enable();

  void run();

  static void task(void*);

};
