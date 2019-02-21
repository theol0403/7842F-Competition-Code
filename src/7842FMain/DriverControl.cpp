#include "DriverControl.hpp"

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

static ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;

void driverIntakeControl()
{
	/**
	* Intake
	*/
	if(j_Digital(R1) && j_Digital(L1))
	{
		intakeState = IntakeController::outSlow;
	}
	else if(j_Digital(R2))
	{
		intakeState = IntakeController::loading;
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
		robot.shooter->doMacro(shootMacro);
		lastShootMacro = shootMacro;
	}

}


void driverFlywheelControl()
{

	if(j_Digital(left))
	{
		wantedRpm = 2350;
		triggerUpdate = true;
	}
	else if(j_Digital(up))
	{
		wantedRpm = 2500;
		triggerUpdate = true;
	}
	else if(j_Digital(right))
	{
		wantedRpm = 2800;
		triggerUpdate = true;
	}
	else if(j_Digital(down))
	{
		wantedRpm = 0;
		triggerUpdate = true;
	}

	if(triggerUpdate) //If new flywheel speed was set
	{
		robot.flywheel->setRpm(wantedRpm);
		triggerUpdate = false;
	}

}
