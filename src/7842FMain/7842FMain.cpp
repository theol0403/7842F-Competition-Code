#include "main.h"

#include "MotorConfig.hpp"
#include "FlywheelController.hpp"
#include "IntakeController.hpp"
#include "lib7842/lib7842.hpp"

#include "DriverControl.hpp"

#include "7842FMain/Auton/AutonFunctions.hpp"


/***   _____         _
*    |_   _|       | |
*      | | __ _ ___| | _____
*      | |/ _` / __| |/ / __|
*      | | (_| \__ \   <\__ \
*      \_/\__,_|___/_|\_\___/
* Checks current task state and changes it if the state does not match the wanted state
* @param taskPtr  pointer to the task
* @param taskMode one of two options, TASK_STATE_SUSPENDED or TASK_STATE_RUNNING
*/
void setTaskState(pros::Task* taskPtr, pros::task_state_e_t taskMode) {
  if(taskPtr != nullptr && taskPtr->get_state() != taskMode) { switch(taskMode) {
    case TASK_STATE_SUSPENDED: taskPtr->suspend(); break;
    case TASK_STATE_RUNNING: taskPtr->resume(); break;
    default: {}
  }}}


  lib7842::AutonSelector *autonSelector;


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
    pros::delay(500); //Give the legacy ports time to start up

    autonSelector = new lib7842::AutonSelector(lv_scr_act(), {{"Test", AutonTest}, {"Close", AutonClose}});

    initializeBase();
    initializeDevices();
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
    //Tries to turn everything off
    //setFlywheelRPM(0);
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
    checkBaseStatus();
    robot.model->stop();

    while(true)
    {

      if(j_Digital(A)) { autonomous(); }

      if(j_Digital(B)) {
        robot.tracker->resetState();
        robot.tracker->resetSensors();
      }

      double rightY = j_Analog(rightY);
      double leftX = j_Analog(leftX);

      robot.model->arcade(rightY, leftX, 0);

      #ifndef TEST_ROBOT
      driverIntakeControl();
      driverFlywheelControl();
      #endif

      pros::delay(20);
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
    SideController* sideChassis = new SideController(robot.chassis, autonSelector->getSelectedSide());
    autonSelector->getSelectedAuton().autonFunc(sideChassis);
    delete sideChassis;
    std::cout << "Exit Auton" << std::endl;
  }
