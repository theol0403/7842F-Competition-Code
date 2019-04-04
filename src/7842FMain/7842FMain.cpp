

#include "lib7842/auton/autonSelector.hpp"
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
  display.main->splashScreen(&img_navigators, 2000);

  display.selector = new lib7842::AutonSelector(display.main->newTab("Auton"), {
    {"T", AutonTest}, {"C", AutonClose},
    {"Mc", AutonMiddleFromClose}, {"Mf", AutonMiddleFromFar},
    {"F", AutonFar}, {"Pf", AutonPlatformFar}
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
  robot.shooter->doJob(ShootController::off);
  robot.flywheel->disable();
  robot.flywheel->resetSlew();
  robot.arm->setState(ArmController::off);
  #endif
  robot.model->stop();
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
  robot.model->stop();

  #ifndef TEST_ROBOT //This resets all the subsystems
  robot.shooter->clearQueue();
  robot.flywheel->resetSlew();
  robot.flywheel->enable();
  robot.intake->setState(IntakeController::off);
  robot.arm->setState(ArmController::off);
  #endif

  robot.printer->rumble(".");

  Timer opTimer;
  opTimer.placeMark();

  while(true) {

    if(j_Digital(A)) {
      autonomous();
    }

    double rightY = j_Analog(rightY);
    double leftX = j_Analog(leftX);
    robot.model->arcade(rightY, ipow(std::abs(leftX), 3) * sgn(leftX), 0);

    #ifndef TEST_ROBOT
    driverControl();
    #endif

    QTime remaining = 1.75_min - opTimer.getDtFromMark();
    if(remaining < 0_ms) {
      robot.printer->print(0, std::to_string((int)(opTimer.getDtFromMark().convert(second))) + "   " + std::to_string((int)(pros::c::battery_get_capacity())) + "%");
    } else if(remaining > 1_min) {
      robot.printer->print(0, std::to_string((int)(remaining.convert(minute))) + ":" + std::to_string((int)((remaining - 1_min).convert(second))) + "  " + std::to_string((int)(pros::c::battery_get_capacity())) + "%");
    } else {
      robot.printer->print(0, std::to_string((int)(remaining.convert(second))) + "  " + std::to_string((int)(pros::c::battery_get_capacity())) + "%");
    }

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
  robot.shooter->clearQueue();
  robot.flywheel->resetSlew();
  robot.flywheel->enable();
  robot.flywheel->setRpm(globalFlywheelRPM);
  //robot.arm->setState(ArmController::unfold);
  #endif

  //Create a new chassis that mirrors side and send it to the autonomous code

  AutonPasser passer = std::make_tuple(
    SideController(robot.chassis, display.selector->getSelectedSide()),
    Timer()
  );

  display.selector->getSelectedAuton().autonFunc(&passer);

  std::cout << "Exit Auton" << std::endl;
}
