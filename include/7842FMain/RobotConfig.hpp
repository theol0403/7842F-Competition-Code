#pragma once
#include "main.h"

#include "lib7842/auton/autonSelector.hpp"
#include "lib7842/odometry/controller.hpp"
#include "lib7842/other/controllerPrinter.hpp"

#include "Controllers/IntakeController.hpp"
#include "Controllers/FlywheelController.hpp"
#include "Controllers/ShootController.hpp"
#include "Controllers/ArmController.hpp"
#include "Controllers/VisionController.hpp"

#include "Display/MainDisplay.hpp"
#include "Display/FlywheelTuner.hpp"
#include "Display/Graph.hpp"
#include "Display/OdomDisplay.hpp"
#include "Display/AngleTuner.hpp"

#define j_Digital(x) robot.joystick->getDigital(okapi::ControllerDigital::x)
#define j_Analog(x) robot.joystick->getAnalog(okapi::ControllerAnalog::x)

using namespace lib7842;

struct display_t
{
  MainDisplay* main = nullptr;
  lib7842::AutonSelector* selector = nullptr;
  FlywheelTuner* flywheel = nullptr;
  Graph* graph = nullptr;
  OdomDisplay* odom = nullptr;
  AngleTuner* angler = nullptr;
};

struct robot_t
{
  okapi::Controller* joystick = nullptr;
  ControllerPrinter* printer = nullptr;

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
