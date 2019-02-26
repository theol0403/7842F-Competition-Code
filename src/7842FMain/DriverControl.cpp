#include "DriverControl.hpp"

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

static ArmController::armStates armState = ArmController::off;
static ArmController::armStates lastArmState = ArmController::off;

static okapi::ControllerButton flywheelTrigger = j_Main[ControllerDigital::down];

static ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;


void driverControl()
{
	/**
	* Intake Control
	*/
	if(j_Digital(R2)) {
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


	if(j_Digital(B)) {
		armState = ArmController::down;
	} else if(j_Digital(Y)) {
		armState = ArmController::out;
	} else if(j_Digital(X)) {
		armState = ArmController::up;
	}

	if(armState != lastArmState) {
		robot.arm->setState(armState);
		lastArmState = armState;
	}


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

		robot.arm->setState(ArmController::off);
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
		//shootMacro = ShootController::shootMacros::shootTopFlag;
		shootMacro = ShootController::shootMacros::shootTarget;
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
	if(j_Digital(left))
	{
		robot.tracker->setY(9_ft);
		robot.tracker->setTheta(0_deg);
	}
	else if(j_Digital(up))
	{
		robot.tracker->setY(6_ft);
		robot.tracker->setTheta(0_deg);
	}
	else if(j_Digital(right))
	{
		robot.tracker->setY(0_ft);
		robot.tracker->setTheta(0_deg);
	}

	if(j_Digital(B)) {
		robot.tracker->setState({0_ft, 7_ft, 0_deg});
	}

	robot.shooter->setDistanceToFlag(11_ft - robot.tracker->state.y);


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
