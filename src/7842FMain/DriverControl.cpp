#include "DriverControl.hpp"

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

static ArmController::armStates armState = ArmController::off;
static ArmController::armStates lastArmState = ArmController::off;

static okapi::ControllerButton flywheelTrigger = j_Main[ControllerDigital::B];

static ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;

QLength distanceToFlag = 0_ft;

void driverControl()
{
	/**
	* Intake Control
	*/
	if(j_Digital(R2) && j_Digital(R1)) {
		intakeState = IntakeController::outSlow;
	} else if(j_Digital(R2)) {
		intakeState = IntakeController::intakeBall;
	} else if(j_Digital(R1)) {
		intakeState = IntakeController::outIntake;
	} else {
		intakeState = IntakeController::off;
	}

	if(intakeState != lastIntakeState) {
		robot.intake->setState(intakeState);
		lastIntakeState = intakeState;
	}


	/**
	* Arm Control
	*/
	// if(j_Digital(B)) {
	// 	armState = ArmController::down;
	// } else if(j_Digital(Y)) {
	// 	armState = ArmController::out;
	// } else if(j_Digital(X)) {
	// 	armState = ArmController::up;
	// }
	//
	// if(armState != lastArmState) {
	// 	robot.arm->setState(armState);
	// 	lastArmState = armState;
	// }


	/**
	* Flywheel Toggle
	* Angling Abort
	* Arm Abort
	*/
	if(flywheelTrigger.changedToPressed()) {
		if(robot.flywheel->getTargetRpm() == 0) {
			robot.flywheel->setRpm(globalFlywheelRPM);
		} else {
			robot.flywheel->setRpm(0);
		}
		shootMacro = ShootController::shootMacros::off;
		lastShootMacro = ShootController::shootMacros::off;
		robot.shooter->doJob(ShootController::off);

		//robot.arm->setState(ArmController::off);
		armState = ArmController::off;
		lastArmState = ArmController::off;
	}

	/**
	* Automatic Control
	* Arrow buttons set odom y
	* Angle of hood is calculated from y using lookup table
	* Pressing one of the two shoot buttons (representing flag) will drop the hood to the proper angle and shoot
	*/
	/**
	* Shoot Control
	*/
	if(j_Digital(L2)) {
		shootMacro = ShootController::shootMacros::shootMiddleFlag;
	} else if(j_Digital(L1)) {
		shootMacro = ShootController::shootMacros::shootTopFlag;
	} else {
		shootMacro = ShootController::shootMacros::off;
	}

	if(shootMacro != lastShootMacro)
	{
		if(shootMacro != ShootController::shootMacros::off) robot.shooter->doMacro(shootMacro);
		lastShootMacro = shootMacro;
	}


	/**
	* Angle Control
	*/

		if(j_Digital(down))
	{
		distanceToFlag = 2_ft;
	}
	else if(j_Digital(left))
	{
		distanceToFlag = 4_ft;
	}
	else if(j_Digital(up))
	{
		distanceToFlag = 5_ft;
	}
	else if(j_Digital(right))
	{
		distanceToFlag = 11_ft;
	}

	robot.shooter->setDistanceToFlag(distanceToFlag);


	// if(j_Digital(L2))
	// {
	// 	shootMacro = ShootController::shootMacros::shoot;
	// }
	// else if(j_Digital(L1))
	// {
	// 	shootMacro = ShootController::shootMacros::angleManual;
	// }
	// else
	// {
	// 	shootMacro = ShootController::shootMacros::off;
	// }
	//
	// if(shootMacro != lastShootMacro)
	// {
	// 	robot.shooter->doMacroLoop(shootMacro);
	// 	lastShootMacro = shootMacro;
	// }


}
