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



static int wantedFlywheelSpeed = 0; // Contains wanted RPM or Power Depending on mode
static bool triggerUpdate = true; // Tells code to send new speed to the flywheel

static bool flywheelManual = false; // Toggle whether RPM control is used or manual Power
//static bool togglePressed = false; // Used to compare button state to last to see if it has been pressed

void driverFlywheelControl()
{
	//
	// if(j_Main.getDigital(okapi::ControllerDigital::left))
	// {
	// 	wantedFlywheelSpeed = flywheelManual ? 50 : 2350;
	// 	triggerUpdate = true;
	// }
	// else if(j_Main.getDigital(okapi::ControllerDigital::up))
	// {
	// 	wantedFlywheelSpeed = flywheelManual ? 70 : 2500;
	// 	triggerUpdate = true;
	// }
	// else if(j_Main.getDigital(okapi::ControllerDigital::right))
	// {
	// 	wantedFlywheelSpeed = flywheelManual ? 90 : 2800;
	// 	triggerUpdate = true;
	// }
	// else if(j_Main.getDigital(okapi::ControllerDigital::down))
	// {
	// 	wantedFlywheelSpeed = 0;
	// 	triggerUpdate = true;
	// }
	//
	// if(triggerUpdate) //If new flywheel speed was set
	// {
	// 	if(flywheelManual)
	// 	{
	// 		setFlywheelRPM(0);
	// 		setFlywheelPower(wantedFlywheelSpeed);
	// 	}
	// 	else
	// 	{
	// 		setFlywheelRPM(wantedFlywheelSpeed);
	// 	}
	// 	triggerUpdate = false;
	// }
	//
	//
	// if(j_Main.getDigital(okapi::ControllerDigital::X))
	// {
	// 	setFlywheelArmMode(true);
	// }
	// else
	// {
	// 	setFlywheelArmMode(false);
	// }
}
