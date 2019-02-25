#include "DriverControl.hpp"

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

static ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;

static okapi::ControllerButton flywheelTrigger = j_Main[ControllerDigital::down];
static double targetAngle = 0;

void driverControl()
{
	/**
	* Intake Control
	*/
	if(j_Digital(R2))
	{
		intakeState = IntakeController::intakeBall;
	}
	else if(j_Digital(R1))
	{
		intakeState = IntakeController::outIntake;
	}
	else
	{
		intakeState = IntakeController::off;
	}

	if(intakeState != lastIntakeState)
	{
		robot.intake->setState(intakeState);
		lastIntakeState = intakeState;
	}

	/**
	* Shoot Control
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
		if(shootMacro == ShootController::shootMacros::shootTarget) std::cout << "Shot at Y : " << robot.tracker->state.y.convert(foot) << " at Angle : " << targetAngle << std::endl;
		if(shootMacro != ShootController::shootMacros::off) robot.shooter->doMacro(shootMacro);
		lastShootMacro = shootMacro;
	}

	/**
	* Shoot Controller
	* Manual Angling
	*/
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


	/**
	* Angle Control
	*/
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

	robot.shooter->setDistanceToFlag(11_ft - robot.tracker->state.y);


	/**
	* Flywheel Control
	* Flywheel Toggle
	* Angling Abort
	*/
	if(flywheelTrigger.changedToPressed()) {
		if(robot.flywheel->getTargetRpm() == 0) {
			robot.flywheel->setRpm(globalFlywheelRPM);
		} else {
			robot.flywheel->setRpm(0);
		}
		robot.shooter->doJob(ShootController::off);
		robot.arm->setState(ArmController::unfold); //for testing
	}

}
