#include "DriverControl.hpp"

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

void driverIntakeControl()
{
	if(j_Digital(R2) && j_Digital(L2))
	{
		intakeState = IntakeController::shootBoth;
	}
	else if(j_Digital(R1) && j_Digital(L1))
	{
		intakeState = IntakeController::outSlow;
	}
	else if(j_Digital(R2))
	{
		intakeState = IntakeController::loading;
	}
	else if(j_Digital(L2))
	{
		intakeState = IntakeController::shootIndexer;
	}
	else if(j_Digital(L1))
	{
		intakeState = IntakeController::outIntake;
	}
	else if(j_Digital(R1))
	{
		intakeState = IntakeController::outBoth;
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

}



static int wantedRpm = 0; // Contains wanted RPM or Power Depending on mode
static bool triggerUpdate = true; // Tells code to send new speed to the flywheel

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
