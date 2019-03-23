#pragma once
#include "main.h"
#include "lib7842/lib7842.hpp"

#include "Controllers/IntakeController.hpp"
#include "Controllers/FlywheelController.hpp"
#include "Controllers/ShootController.hpp"
#include "Controllers/ArmController.hpp"
#include "Controllers/VisionController.hpp"

#include "Display/MainDisplay.hpp"
#include "Display/FlywheelTuner.hpp"
#include "Display/Graph.hpp"
#include "Display/OdomDisplay.hpp"

extern okapi::Controller j_Main;
#define j_Digital(x) j_Main.getDigital(okapi::ControllerDigital::x)
#define j_Analog(x) j_Main.getAnalog(okapi::ControllerAnalog::x)


struct display_t
{
  MainDisplay* main = nullptr;
  AutonSelector* selector = nullptr;
  FlywheelTuner* flywheel = nullptr;
  Graph* graph = nullptr;
  OdomDisplay* odom = nullptr;
};

struct robot_t
{
  std::shared_ptr<okapi::SkidSteerModel> model = nullptr;
  lib7842::OdomController* chassis = nullptr;
  lib7842::OdomTracker* tracker = nullptr;

  IntakeController* intake = nullptr;
  FlywheelController* flywheel = nullptr;
  ShootController* shooter = nullptr;
  ArmController* arm = nullptr;
  VisionController* vision = nullptr;
};

extern display_t display;
extern robot_t robot;

extern const int globalFlywheelRPM;

void initializeDevices();
void initializeBase();

//#define TEST_ROBOT
