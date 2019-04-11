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

#define mDigital(x) pros::c::controller_get_digital(pros::E_CONTROLLER_MASTER, pros::E_CONTROLLER_DIGITAL_##x)
#define mDigitalPressed(x) pros::c::controller_get_digital_new_press(pros::E_CONTROLLER_MASTER, pros::E_CONTROLLER_DIGITAL_##x)
#define mAnalog(x) (float)pros::c::controller_get_analog(pros::E_CONTROLLER_MASTER, pros::E_CONTROLLER_ANALOG_##x)/127.0

#define pDigital(x) pros::c::controller_get_digital(pros::E_CONTROLLER_PARTNER, pros::E_CONTROLLER_DIGITAL_##x)
#define pDigitalPressed(x) pros::c::controller_get_digital_new_press(pros::E_CONTROLLER_PARTNER, pros::E_CONTROLLER_DIGITAL_##x)
#define pAnalog(x) (float)pros::c::controller_get_analog(pros::E_CONTROLLER_PARTNER, pros::E_CONTROLLER_ANALOG_##x)/127.0

#define subsystem(x) if(robot.x)robot.x

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
  //ControllerPrinter* pPrinter = nullptr;

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
//#define B_ROBOT
#define F_ROBOT
