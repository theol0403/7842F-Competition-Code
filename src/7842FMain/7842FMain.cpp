#include "main.h"

#include "lib7842/lib7842.hpp"
#include "MotorConfig.hpp"

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
  }}
}


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

  initializeBase();
  initializeDevices();

  autonSelector = new lib7842::AutonSelector(robot.display->newTab("Auton"), {
    {"N", AutonNothing}, {"C", AutonClose}, {"CwP", AutonCloseWithoutPush}, {"Cex", AutonCloseExperimental}, {"Cmid", AutonCloseMiddle},
    {"Mc", AutonMiddleFromClose}, {"Mf", AutonMiddleFromFar},
    {"F", AutonFar}, {"Pf", AutonPlatformFar}
  });
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

  // PIDScreenTuner::pidTune_t flywheelPIDParams = {0.072, 0.0, 0.105, 0.039, 0.15, 0.1};
  // //kP, kI, kD, kF, readingEma, derivativeEma
  // double wantedFlywheelRPM = 0;
  //
  // PIDScreenTuner tuneFlywheel(LV_HOR_RES, LV_VER_RES, LV_VER_RES/3);
  // tuneFlywheel.initButton(0, &flywheelPIDParams.kP, "kP", 5);
  // tuneFlywheel.initButton(65, &flywheelPIDParams.kD, "kD", 5);
  // tuneFlywheel.initButton(130, &flywheelPIDParams.kF, "kF", 5);
  // tuneFlywheel.initButton(195, &flywheelPIDParams.derivativeEma, "DE", 5);
  // tuneFlywheel.initButton(260, &flywheelPIDParams.readingEma, "RE", 5);
  // tuneFlywheel.initButton(325, &wantedFlywheelRPM, "RPM", 4, PIDScreenTuner::buttonIncrement, 300);
  // tuneFlywheel.initButton(400, &tuneFlywheel.m_buttonMultiplier, "Multiplier", 6, PIDScreenTuner::buttonMultiply, 10);
  // lv_obj_t* rpmGauge = tuneFlywheel.initGauge(0, "RPM", 2, 0, 3000);
  // lv_obj_t* errorGauge = tuneFlywheel.initGauge(160, "Error", 1, -50, 50);
  // lv_obj_t* powerGauge = tuneFlywheel.initGauge(320, "MotorPower", 1, 0, 127);

  // lib7842::SDLogger shootLogger("shotLog", lib7842::SDLogger::count);
  // shootLogger.writeFields({"Flag", "Distance", "Angle", "Rpm", "Battery", "Temp"});

  lib7842::SDLogger shootLogger("testLog", lib7842::SDLogger::count);
  shootLogger.writeFields({"Test"});

  // double targetAngle = 0;
  // bool topFlag = true;
  // double shotRpm = 0;
  // okapi::ControllerButton printTrigger = j_Main[ControllerDigital::A];
  // ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
  // ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;

  checkBaseStatus(); //Make sure the base has been initialized properly
  robot.model->stop();

  #ifndef TEST_ROBOT //This resets all the subsystems
  robot.shooter->clearQueue();
  robot.flywheel->resetSlew();
  robot.flywheel->enable();
  robot.intake->setState(IntakeController::off);
  robot.arm->setState(ArmController::off);
  #endif

  while(true) {
    //if(j_Digital(A)) autonomous(); //For testing

    double rightY = j_Analog(rightY);
    double leftX = j_Analog(leftX);
    robot.model->arcade(rightY, ipow(std::abs(leftX), 2) * sgn(leftX), 0);

    #ifndef TEST_ROBOT
    driverControl();
    #endif

    // robot.flywheel->pid->setGains(flywheelPIDParams.kP, flywheelPIDParams.kD, flywheelPIDParams.kF, flywheelPIDParams.derivativeEma);
    // robot.flywheel->rpmFilter->setGains(flywheelPIDParams.readingEma);
    //
    // lv_gauge_set_value(rpmGauge, 0, wantedFlywheelRPM);
    // lv_gauge_set_value(rpmGauge, 1, robot.flywheel->currentRpm);
    // lv_gauge_set_value(errorGauge, 0, -robot.flywheel->pid->getError());
    // lv_gauge_set_value(powerGauge, 0, robot.flywheel->motorPower);
    //
    // robot.flywheel->setRpm(wantedFlywheelRPM);

    //
    // if(j_Digital(L2)) {
    //   shootMacro = ShootController::shootMacros::shootTarget;
    //   topFlag = false;
    //   shotRpm = robot.flywheel->currentRpm;
    // } else if(j_Digital(L1)) {
    //   shootMacro = ShootController::shootMacros::shootTarget;
    //   topFlag = true;
    //   shotRpm = robot.flywheel->currentRpm;
    // } else {
    //   shootMacro = ShootController::shootMacros::off;
    // }
    //
    // if(shootMacro != lastShootMacro)
    // {
    //   if(shootMacro == ShootController::shootMacros::shootTarget)
    //   std::cout << "" << (11_ft - robot.tracker->state.y).convert(foot) << ", " << targetAngle << std::endl;
    //   if(shootMacro != ShootController::shootMacros::off) robot.shooter->doMacro(shootMacro);
    //   lastShootMacro = shootMacro;
    // }
    //
    // if(j_Digital(Y))
    // {
    //   targetAngle -= 0.1;
    //   std::cout << "Target Angle: " << targetAngle << std::endl;
    //   robot.shooter->setTarget(targetAngle);
    // }
    // else if(j_Digital(X))
    // {
    //   targetAngle += 0.1;
    //   std::cout << "Target Angle: " << targetAngle << std::endl;
    //   robot.shooter->setTarget(targetAngle);
    // }
    // else if(printTrigger.changedToPressed())
    // {
    //   //print angle and distance
    // shootLogger.writeLine({
    //   topFlag ? "Top" : "Middle",
    //   std::to_string((11_ft - robot.tracker->state.y).convert(foot)),
    //   std::to_string(targetAngle),
    //   std::to_string(shotRpm),
    //   std::to_string(pros::battery::get_capacity()),
    //   std::to_string(robot.flywheel->flywheel->getTemperature())
    // });
    // }
    //
    // if(j_Digital(B)) {
    //   robot.shooter->doJobLoop(ShootController::angleTarget);
    // }
    shootLogger.writeLine({"Yayyy"});
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

  //Create a new chassis that automatically mirrors side and send it to the autonomous code
  SideController* sideChassis = new SideController(
    robot.chassis, autonSelector->getSelectedSide());
    autonSelector->getSelectedAuton().autonFunc(sideChassis);
    delete sideChassis;
    std::cout << "Exit Auton" << std::endl;
  }
