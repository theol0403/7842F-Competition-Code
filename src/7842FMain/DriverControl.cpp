#include "DriverControl.hpp"

static IntakeController::intakeStates intakeState = IntakeController::standby;
static IntakeController::intakeStates lastIntakeState = IntakeController::standby;

static ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;

void driverIntakeControl()
{
	/**
	* Intake
	*/
	if(j_Digital(R2))
	{
		intakeState = IntakeController::intakeAll;
	}
	else if(j_Digital(R1))
	{
		intakeState = IntakeController::outIntake;
	}
	else
	{
		intakeState = IntakeController::standby;
	}

	if(intakeState != lastIntakeState)
	{
		robot.intake->setState(intakeState);
		lastIntakeState = intakeState;
	}


	/**
	* Shoot Controller
	*/
	if(j_Digital(L2) && j_Digital(L1))
	{
		shootMacro = ShootController::shootMacros::shootBothFlags;
	}
	else if(j_Digital(L2))
	{
		shootMacro = ShootController::shootMacros::shootMiddleFlag;
	}
	else if(j_Digital(L1))
	{
		//shootMacro = ShootController::shootMacros::shootTopFlag;
		shootMacro = ShootController::shootMacros::shootTarget;
	}
	else
	{
		shootMacro = ShootController::shootMacros::off;
	}

	if(shootMacro != lastShootMacro)
	{
		if(shootMacro != ShootController::shootMacros::off) robot.shooter->doMacro(shootMacro);
		lastShootMacro = shootMacro;
	}

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

okapi::ControllerButton flywheelTrigger = j_Main[ControllerDigital::down];
double targetAngle = 0;

void driverFlywheelControl()
{
	if(flywheelTrigger.changedToPressed()) {
		if(robot.flywheel->getTargetRpm() == 0) {
			robot.flywheel->setRpm(globalFlywheelRPM);
		} else {
			robot.flywheel->setRpm(0);
		}
	}

	if(j_Digital(Y))
	{
		targetAngle -= 1;
		robot.shooter->setTarget(targetAngle);
		std::cout << "Target Angle: " << targetAngle << std::endl;
	}
	else if(j_Digital(X))
	{
		targetAngle += 1;
		robot.shooter->setTarget(targetAngle);
		std::cout << "Target Angle: " << targetAngle << std::endl;
	}

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

}
