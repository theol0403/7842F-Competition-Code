#include "DriverFunctions.hpp"


void driverIntakeControl()
{
	if(j_Main.getDigital(okapi::ControllerDigital::R2))
	{
		runIntake = true;
	}
	else
	{
		runIntake = false;
	}

	// if(j_Main.getDigital(okapi::ControllerDigital::R1))
	// {
	// 	setIntakePower(127);
	// }
	// else if(j_Main.getDigital(okapi::ControllerDigital::R2))
	// {
	// 	setIntakePower(-127);
	// }
	// else
	// {
	// 	setIntakePower(0);
	// }
	//
	// if(j_Main.getDigital(okapi::ControllerDigital::L1))
	// {
	// 	setIndexerPower(127);
	// }
	// else if(j_Main.getDigital(okapi::ControllerDigital::L2))
	// {
	// 	setIndexerPower(-127);
	// }
	// else
	// {
	// 	setIndexerPower(0);
	// }
}



static int wantedFlywheelSpeed = 0; // Contains wanted RPM or Power Depending on mode
static bool triggerUpdate = true; // Tells code to send new speed to the flywheel

static bool flywheelManual = false; // Toggle whether RPM control is used or manual Power
static bool togglePressed = false; // Used to compare button state to last to see if it has been pressed

void driverFlywheelControl()
{

	if(j_Main.getDigital(okapi::ControllerDigital::Y) && !togglePressed) // Is button pressed and not before?
	{
		togglePressed = true;
		flywheelManual = flywheelManual ? false : true; //Switch flywheel mode
	}
	else if(!j_Main.getDigital(okapi::ControllerDigital::Y) && togglePressed) // Is button not pressed but it was before?
	{
		togglePressed = false;
	}


	if(j_Main.getDigital(okapi::ControllerDigital::left))
	{
		wantedFlywheelSpeed = flywheelManual ? 50 : 2400;
		triggerUpdate = true;
	}
	else if(j_Main.getDigital(okapi::ControllerDigital::up))
	{
		wantedFlywheelSpeed = flywheelManual ? 70 : 2600;
		triggerUpdate = true;
	}
	else if(j_Main.getDigital(okapi::ControllerDigital::right))
	{
		wantedFlywheelSpeed = flywheelManual ? 90 : 2800;
		triggerUpdate = true;
	}
	else if(j_Main.getDigital(okapi::ControllerDigital::down))
	{
		wantedFlywheelSpeed = 0;
		triggerUpdate = true;
	}

	if(triggerUpdate) //If new flywheel speed was set
	{
		if(flywheelManual)
		{
			setFlywheelRPM(0);
			setFlywheelPower(wantedFlywheelSpeed);
		}
		else
		{
			setFlywheelRPM(wantedFlywheelSpeed);
		}
		triggerUpdate = false;
	}


	if(j_Main.getDigital(okapi::ControllerDigital::X))
	{
		setFlywheelArmMode(true);
	}
	else
	{
		setFlywheelArmMode(false);
	}
}
