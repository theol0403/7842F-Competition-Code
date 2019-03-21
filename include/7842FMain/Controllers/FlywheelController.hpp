#pragma once
#include "main.h"
#include "lib7842/pid/pidVelSystem.hpp"
#include "IntakeController.hpp"
#include "lib7842/other/sdLogger.hpp"

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

  pros::Task flywheelTask;

  double targetRpm = 0;
  double currentRpm = 0;

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
  static void task(void*);

};
