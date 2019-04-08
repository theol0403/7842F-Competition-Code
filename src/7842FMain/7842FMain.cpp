
#include "RobotConfig.hpp"

#include "7842FMain/Auton/AutonIncludes.hpp"
#include "DriverControl.hpp"

/***
*     _____      _ _   _       _ _
*    |_   _|    (_) | (_)     | (_)
*      | | _ __  _| |_ _  __ _| |_ _______
*      | || '_ \| | __| |/ _` | | |_  / _ \
*     _| || | | | | |_| | (_| | | |/ /  __/
*     \___/_| |_|_|\__|_|\__,_|_|_/___\___|
* Runs initialization code. This occurs as soon as the program is started.
*
* All other competition modes are blocked by initialize; it is recommended
* to keep execution time for this mode under a few seconds.
*/
void initialize()
{
  std::cout << "startup heap:    " << xPortGetFreeHeapSize() << std::endl;

  display.main = new MainDisplay(lv_scr_act(), LV_COLOR_HEX(0xFF7F00));
  LV_IMG_DECLARE(img_navigators);
  display.main->splashScreen(&img_navigators, 1500);

  display.selector = new lib7842::AutonSelector(display.main->newTab("Auton"), {
    {"T", AutonTest}, {"C", AutonClose}, {"Ex", AutonCloseExperimental},
    {"Mc", AutonMiddleFromClose}, {"Mf", AutonMiddleFromFar},
    {"F", AutonFar}, {"Pf", AutonPlatformFar}, {"S", AutonSkills}
  });

  pros::delay(500); //Give the legacy ports time to start up

  initializeBase();
  initializeDevices();
  std::cout << "initialized heap: " << xPortGetFreeHeapSize() << std::endl;

}

/***
*     _____                           _____      _ _
*    /  __ \                         |_   _|    (_) |
*    | /  \/ ___  _ __ ___  _ __ ______| | _ __  _| |_
*    | |    / _ \| '_ ` _ \| '_ \______| || '_ \| | __|
*    | \__/\ (_) | | | | | | |_) |    _| || | | | | |_
*     \____/\___/|_| |_| |_| .__/     \___/_| |_|_|\__|
*                          |_|
* Runs after initialize(), and before autonomous when connected to the Field
* Management System or the VEX Competition Switch. This is intended for
* competition-specific initialization routines, such as an autonomous selector
* on the LCD.
*
* This task will exit when the robot is enabled and autonomous or opcontrol
* starts.
*/
void competition_initialize() {}

/***
*    ______ _           _     _          _
*    |  _  (_)         | |   | |        | |
*    | | | |_ ___  __ _| |__ | | ___  __| |
*    | | | | / __|/ _` | '_ \| |/ _ \/ _` |
*    | |/ /| \__ \ (_| | |_) | |  __/ (_| |
*    |___/ |_|___/\__,_|_.__/|_|\___|\__,_|
* Runs while the robot is in the disabled state of Field Management System or
* the VEX Competition Switch, following either autonomous or opcontrol. When
* the robot is enabled, this task will exit.
*/
void disabled()
{
  #ifndef TEST_ROBOT
  subsystem(shooter)->doJob(ShootController::off);
  subsystem(flywheel)->disable();
  subsystem(flywheel)->resetSlew();
  subsystem(arm)->setState(ArmController::off);
  #endif
  subsystem(model)->stop();
}

/***
*     _   _                              _             _
*    | | | |                            | |           | |
*    | | | |___  ___ _ __ ___ ___  _ __ | |_ _ __ ___ | |
*    | | | / __|/ _ \ '__/ __/ _ \| '_ \| __| '__/ _ \| |
*    | |_| \__ \  __/ | | (_| (_) | | | | |_| | | (_) | |
*     \___/|___/\___|_|  \___\___/|_| |_|\__|_|  \___/|_|
* Runs the operator control code. This function will be started in its own task
* with the default priority and stack size whenever the robot is enabled via
* the Field Management System or the VEX Competition Switch in the operator
* control mode.
*
* If no competition control is connected, this function will run immediately
* following initialize().
*
* If the robot is disabled or communications is lost, the
* operator control task will be stopped. Re-enabling the robot will restart the
* task, not resume it from where it left off.
*/
void opcontrol()
{
  subsystem(model)->stop();

  #ifndef TEST_ROBOT //This resets all the subsystems
  subsystem(shooter)->clearQueue();
  subsystem(flywheel)->resetSlew();
  subsystem(flywheel)->enable();
  subsystem(intake)->setState(IntakeController::off);
  subsystem(arm)->setState(ArmController::off);
  #endif

  if(display.selector->m_currentAutonIndex != 0) {
    subsystem(flywheel)->setRpm(globalFlywheelRPM);
  }

  robot.mPrinter->rumble("-");

  Timer opTimer;
  opTimer.placeMark();

  while(true) {

    // if(mDigital(A)) {
    //   robot.mPrinter->rumble("--");
    //   autonomous();
    // }

    double rightY = mAnalog(rightY);
    double leftX = mAnalog(leftX);
    subsystem(model)->arcade(rightY, ipow(std::abs(leftX), 3) * sgn(leftX), 0);

    #ifndef TEST_ROBOT
    driverControl();
    #endif

    QTime remaining = 1.75_min - opTimer.getDtFromMark();
    if(remaining < 0_ms) {
      robot.mPrinter->print(0, std::to_string((int)(opTimer.getDtFromMark().convert(second))) + "   " + std::to_string((int)(pros::c::battery_get_capacity())) + "%");
    } else if(remaining > 1_min) {
      robot.mPrinter->print(0, std::to_string((int)(remaining.convert(minute))) + ":" + std::to_string((int)((remaining - 1_min).convert(second))) + "  " + std::to_string((int)(pros::c::battery_get_capacity())) + "%");
    } else {
      robot.mPrinter->print(0, std::to_string((int)(remaining.convert(second))) + "  " + std::to_string((int)(pros::c::battery_get_capacity())) + "%");
    }

    robot.pPrinter->print(0, robot.mPrinter->get(0));
    robot.pPrinter->print(1, robot.mPrinter->get(1));
    robot.pPrinter->print(2, robot.mPrinter->get(2));

    // if(true) {
    //   if(remaining == 60_s) {
    //     j_Main.rumble(".");
    //   } else if(remaining == 30_s) {
    //     j_Main.rumble(".-");
    //   } else if(remaining == 15_s) {
    //     j_Main.rumble("..-");
    //   } else if(remaining == 10_s) {
    //     j_Main.rumble("....");
    //   }
    // }


    pros::delay(10);
  }
}


/***
*      ___        _
*     / _ \      | |
*    / /_\ \_   _| |_ ___
*    |  _  | | | | __/ _ \
*    | | | | |_| | || (_) |
*    \_| |_/\__,_|\__\___/
* Runs the user autonomous code. This function will be started in its own task
* with the default priority and stack size whenever the robot is enabled via
* the Field Management System or the VEX Competition Switch in the autonomous
* mode. Alternatively, this function may be called in initialize or opcontrol
* for non-competition testing purposes.
*
* If the robot is disabled or communications is lost, the autonomous task
* will be stopped. Re-enabling the robot will restart the task, not re-start it
* from where it left off.
*/

void autonomous()
{
  #ifndef TEST_ROBOT
  subsystem(shooter)->clearQueue();
  subsystem(flywheel)->resetSlew();
  subsystem(flywheel)->enable();
  subsystem(flywheel)->setRpm(globalFlywheelRPM);
  //subsystem(arm)->setState(ArmController::unfold);
  #endif

  //Create a new chassis that mirrors side and send it to the autonomous code

  AutonPasser passer = std::make_tuple(
    SideController(robot.chassis, display.selector->getSelectedSide()),
    Timer()
  );

  display.selector->getSelectedAuton().autonFunc(&passer);

  std::cout << "Exit Auton" << std::endl;

  subsystem(flywheel)->setRpm(0);
}
