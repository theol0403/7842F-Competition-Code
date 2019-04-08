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
#include "Display/ChassisTuner.hpp"
#include "Display/DriverDisplay.hpp"

extern okapi::Controller j_Main;
#define mDigital(x) j_Main.getDigital(okapi::ControllerDigital::x)
#define mAnalog(x) j_Main.getAnalog(okapi::ControllerAnalog::x)

extern okapi::Controller j_Partner;
#define pDigital(x) j_Parter.getDigital(okapi::ControllerDigital::x)
#define pAnalog(x) j_Parter.getAnalog(okapi::ControllerAnalog::x)

using namespace lib7842;

struct display_t
{
  MainDisplay* main = nullptr;
  lib7842::AutonSelector* selector = nullptr;
  FlywheelTuner* flywheel = nullptr;
  Graph* graph = nullptr;
  OdomDisplay* odom = nullptr;
  AngleTuner* angler = nullptr;
  DriverDisplay* driverDisplay = nullptr;
  //ChassisTuner* chassisTuner = nullptr;
};

struct robot_t
{
  ControllerPrinter* mPrinter = nullptr;
  ControllerPrinter* pPrinter = nullptr;

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
