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
		shootMacro = ShootController::shootMacros::shootTopFlag;
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

void driverFlywheelControl()
{
	if(flywheelTrigger.changedToPressed()) {
		if(robot.flywheel->getTargetRpm() == 0) {
			robot.flywheel->setRpm(globalFlywheelRPM);
		} else {
			robot.flywheel->setRpm(0);
		}
	}

	//
	// if(j_Digital(left))
	// {
	// 	wantedRpm = 2350;
	// 	triggerUpdate = true;
	// }
	// else if(j_Digital(up))
	// {
	// 	wantedRpm = 2500;
	// 	triggerUpdate = true;
	// }
	// else if(j_Digital(right))
	// {
	// 	wantedRpm = 2800;
	// 	triggerUpdate = true;
	// }
	// else if(j_Digital(down))
	// {
	// 	wantedRpm = 0;
	// 	triggerUpdate = true;
	// }
	//
	// if(triggerUpdate) //If new flywheel speed was set
	// {
	// 	robot.flywheel->setRpm(wantedRpm);
	// 	triggerUpdate = false;
	// }

}
