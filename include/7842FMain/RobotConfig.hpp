#pragma once
#include "main.h"
#include "lib7842/lib7842.hpp"
#include "Controllers/IntakeController.hpp"
#include "Controllers/FlywheelController.hpp"
#include "Controllers/ShootController.hpp"
#include "Controllers/ArmController.hpp"
#include "Controllers/VisionController.hpp"
#include "Controllers/DisplayController.hpp"
#include "7842FMain/Auton/AutonFunctions.hpp"

extern okapi::Controller j_Main;
#define j_Digital(x) j_Main.getDigital(okapi::ControllerDigital::x)
#define j_Analog(x) j_Main.getAnalog(okapi::ControllerAnalog::x)


struct robot_t
{
  std::shared_ptr<okapi::SkidSteerModel> model = nullptr;
  lib7842::OdomController* chassis = nullptr;
  lib7842::OdomTracker* tracker = nullptr;

  DisplayController* display = nullptr;
  IntakeController* intake = nullptr;
  FlywheelController* flywheel = nullptr;
  ShootController* shooter = nullptr;
  ArmController* arm = nullptr;
  AutonSelector* selector = nullptr;
  VisionController* vision = nullptr;
};

extern robot_t robot;
extern const int globalFlywheelRPM;

void initializeDevices();
void initializeBase();
void checkBaseStatus();


//#define TEST_ROBOT