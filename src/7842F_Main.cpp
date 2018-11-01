#include "main.h"

#include "Include/Shared/MotorConfig.hpp"







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

#include "Include/Driver/DriverMainTasks.hpp"
#include "Include/Shared/FlywheelTask.hpp"

#include "Include/FlagTracking/FlagTracking.hpp"

void opcontrol()
{
	pros::Task DriverMainTask_t(DriverMainTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "DriverTask");
pros::Task DriverFlywheelTask_t(flywheelTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "FywheelTajs");

	//pros::Task FlagTrackingTask_t(mainFlagTrackingTask, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "FlagTask");

pros::delay(100000);


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
void autonomous() {}
