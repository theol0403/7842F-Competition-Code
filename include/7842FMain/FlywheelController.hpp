#pragma once
#include "main.h"
#include "lib7842/pid/pidVelSystem.hpp"

class FlywheelController
{

public:

  AbstractMotor* flywheel = nullptr;
  double flywheelRatio = 1;
  lib7842::velPID* pid = nullptr;
  lib7842::emaFilter* rpmFilter;
  double motorSlew;

  pros::Task flywheelTask;

  double targetRPM = 0;
  double currentRPM = 0;
  double slewRPM = 0;

  double lastPower = 0;
  double motorPower = 0;

  bool disabled = false;

  FlywheelController(AbstractMotor*, double, lib7842::velPID*, lib7842::emaFilter*, double);

  void setRpm(double);
  void disable();
  void enable();

  void run();

  static void task(void*);

};
