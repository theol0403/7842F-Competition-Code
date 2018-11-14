#include "main.h"

#include "Include/Shared/MotorConfig.hpp"
#include "Include/Shared/FlywheelTask.hpp"

#include "Include/FlagTracking/FlagTracking.hpp"

#include "Include/Driver/DriverMainTasks.hpp"



/***   _____         _
 *    |_   _|       | |
 *      | | __ _ ___| | _____
 *      | |/ _` / __| |/ / __|
 *      | | (_| \__ \   <\__ \
 *      \_/\__,_|___/_|\_\___/
 */
//Sets task state
 void setTaskState(pros::Task* taskPtr, pros::task_state_e_t taskMode) {
   if(taskPtr->get_state() != taskMode) { //If state is wrong
     switch(taskMode) {
       case TASK_STATE_SUSPENDED: taskPtr->suspend(); break;
       case TASK_STATE_RUNNING: taskPtr->resume(); break;
       default: {} } } }

//Shared
pros::Task MainFlywheelTask_t(flywheelTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT);
//Driver
pros::Task DriverMainTask_t(DriverMainTask, NULL, TASK_PRIORITY_DEFAULT, 0x3000);
//Auton

void compManagerTask(void*)
{
  int currentStatus = COMPETITION_DISABLED;
  while(true)
  {
    currentStatus = pros::competition::get_status();
    if(currentStatus != COMPETITION_DISABLED) //Enabled
    {
      //Shared
      setTaskState(&MainFlywheelTask_t, TASK_STATE_RUNNING);
      if(currentStatus != COMPETITION_AUTONOMOUS) //Driver Control
      {
        //Driver on
        setTaskState(&DriverMainTask_t, TASK_STATE_RUNNING);
        //Auto off
      }
      else if(currentStatus == COMPETITION_AUTONOMOUS) //Autonomous
      {
        //Driver off
        setTaskState(&DriverMainTask_t, TASK_STATE_SUSPENDED);
        //Auto on
      }
    }
    else if(currentStatus != COMPETITION_CONNECTED) {} //Disconnected
    else //Disabled
    {
      //Auton Selector
      //All other tasks off
      setTaskState(&MainFlywheelTask_t, TASK_STATE_SUSPENDED); setFlywheelRPM(0);
      setTaskState(&DriverMainTask_t, TASK_STATE_SUSPENDED);
      //Motors Off
    }

    pros::delay(100);
  }
}

//Manager
pros::Task compManagerTask_t(compManagerTask, NULL, TASK_PRIORITY_DEFAULT+2, TASK_STACK_DEPTH_DEFAULT);



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


	//pros::Task FlagTrackingTask_t(mainFlagTrackingTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "FlagTask");

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
void disabled() {}





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
  while(true) {pros::delay(10000);} //Never exits
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
  DriverMainTask_t.suspend();

}
