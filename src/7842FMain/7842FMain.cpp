
#include "RobotConfig.hpp"

#include "7842FMain/Auton/AutonIncludes.hpp"
#include "DriverControl.hpp"

static bool isCompetition = false; //will be toggled when autonomous is run

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
  display.main->splashScreen(&img_navigators, 1800);

  display.selector = new lib7842::AutonSelector(display.main->newTab("Auton"), {
    {"T", AutonTest}, {"C", AutonClose},
    {"Mc", AutonMiddleFromClose}, {"Mf", AutonMiddleFromFar},
    {"Fd", AutonFarDescore}, {"Fp", AutonFarPost}, {"Fdp", AutonFarPost}
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
void competition_initialize() {
  isCompetition = true;
}

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
  subsystem(chassis)->stop();
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
  subsystem(chassis)->stop();

  #ifndef TEST_ROBOT //This resets all the subsystems
  subsystem(shooter)->clearQueue();
  subsystem(flywheel)->resetSlew();
  subsystem(flywheel)->enable();
  subsystem(intake)->setState(IntakeController::off);
  subsystem(arm)->setState(ArmController::off);
  #endif

  //if it is compeition, automatically start the flywheel
  if(isCompetition) {
    subsystem(flywheel)->setRpm(globalFlywheelRPM);
  }

  robot.printer->rumble(".");

  Timer opTimer;
  opTimer.placeMark();

  while(true) {

    if(mDigital(A)) {
      robot.printer->rumble("-");
      autonomous();
    }

    #ifndef TEST_ROBOT
    driverControl();
    #endif

    std::string str;
    QTime remaining = 1.75_min - opTimer.getDtFromMark();
    if(remaining < 0_ms) {
      str = std::to_string((int)(opTimer.getDtFromMark().convert(second))) + "  " + std::to_string((int)(pros::c::battery_get_capacity())) + "%";
    } else if(remaining > 1_min) {
      str = std::to_string((int)(remaining.convert(minute))) + ":" + std::to_string((int)((remaining - 1_min).convert(second))) + "  " + std::to_string((int)(pros::c::battery_get_capacity())) + "%";
    } else {
      str = std::to_string((int)(remaining.convert(second))) + "  " + std::to_string((int)(pros::c::battery_get_capacity())) + "%";
    }

    robot.printer->print(0, str + "  M", pros::E_CONTROLLER_MASTER);
    robot.printer->print(0, str + "  P", pros::E_CONTROLLER_PARTNER);

    robot.printer->copy(1, pros::E_CONTROLLER_MASTER, pros::E_CONTROLLER_PARTNER);
    robot.printer->copy(2, pros::E_CONTROLLER_MASTER, pros::E_CONTROLLER_PARTNER);

    // if(true) { //isCompetition
    //   if(remaining == 30_s) {
    //     robot.printer->rumble("-");
    //   } else if(remaining == 15_s) {
    //     robot.printer->rumble("--");
    //   } else if(remaining == 10_s) {
    //     robot.printer->rumble("....");
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
  #endif

  isCompetition = true;

  //Create a new chassis that mirrors side and send it to the autonomous code
  //create timer
  AutonPasser passer = std::make_tuple(
    SideController(robot.chassis, display.selector->getSelectedSide()),
    Timer()
  );

  display.selector->getSelectedAuton().autonFunc(&passer);

  subsystem(flywheel)->setRpm(0);
  subsystem(chassis)->stop();
}
