#pragma once
#include "main.h"
#include "lib7842/pid/pidVelSystem.hpp"
#include "IntakeController.hpp"

class FlywheelController
{

public:

  IntakeController*& intake;
  Motor* flywheel = nullptr;
  ADIEncoder* sensor = nullptr;
  VelMath* velMath = nullptr;
  lib7842::emaFilter* rpmFilter = nullptr;
  lib7842::velPID* pid = nullptr;
  double motorSlew;

  pros::Task task;

  double targetRpm = 0;
  double currentRpm = 0;

  double lastRpm = 0;
  QAngularAcceleration currentAccel = 0_rpm / second;
  double rpmPerSecond = 0;

  double lastPower = 0;
  double motorPower = 0;

  bool disabled = false;

  FlywheelController(IntakeController*&, Motor*, ADIEncoder*, VelMath*, lib7842::emaFilter*, lib7842::velPID*, double);

  void setRpm(double);
  double getTargetRpm();
  void disable();
  void enable();
  void resetSlew();

  void run();
  static void taskFnc(void*);

};
