#pragma once
#include "main.h"
#include "lib7842/lib7842.hpp"
#include "IntakeController.hpp"
#include "FlywheelController.hpp"
#include "ShootController.hpp"

extern okapi::Controller j_Main;
#define j_Digital(x) j_Main.getDigital(okapi::ControllerDigital::x)
#define j_Analog(x) j_Main.getAnalog(okapi::ControllerAnalog::x)


struct robot_t
{
  std::shared_ptr<okapi::SkidSteerModel> model = nullptr;
  lib7842::OdomController* chassis = nullptr;
  lib7842::OdomTracker* tracker = nullptr;

  IntakeController* intake = nullptr;
  FlywheelController* flywheel = nullptr;
  ShootController* shoot = nullptr;
};

extern robot_t robot;

void initializeDevices();
void initializeBase();
void checkBaseStatus();


#define TEST_ROBOT
