#include "DriverFunctions.hpp"


void driverIntakeControl()
{

	if(j_Main.getDigital(ControllerDigital::R1))
	{
		setIntakePower(90);
	}
	else if(j_Main.getDigital(ControllerDigital::R2))
	{
		setIntakePower(-127);
	}
	else
	{
		setIntakePower(0);
	}

	if(j_Main.getDigital(ControllerDigital::L1))
	{
		setIndexerPower(127);
	}
	else if(j_Main.getDigital(ControllerDigital::L2))
	{
		setIndexerPower(-127);
	}
	else
	{
		setIndexerPower(0);
	}
}



static int wantedFlywheelSpeed = 0; // Contains wanted RPM or Power Depending on mode
static bool triggerUpdate = true; // Tells code to send new speed to the flywheel

static bool flywheelManual = false; // Toggle whether RPM control is used or manual Power
static bool togglePressed = false; // Used to compare button state to last to see if it has been pressed

void driverFlywheelControl()
{

	if(j_Main.getDigital(ControllerDigital::Y) && !togglePressed) // Is button pressed and not before?
	{
		togglePressed = true;
		flywheelManual = flywheelManual ? false : true; //Switch flywheel mode
	}
	else if(!j_Main.getDigital(ControllerDigital::Y) && togglePressed) // Is button not pressed but it was before?
	{
		togglePressed = false;
	}


	if(j_Main.getDigital(ControllerDigital::left))
	{
		wantedFlywheelSpeed = flywheelManual ? 50 : 2400;
		triggerUpdate = true;
	}
	else if(j_Main.getDigital(ControllerDigital::up))
	{
		wantedFlywheelSpeed = flywheelManual ? 70 : 2600;
		triggerUpdate = true;
	}
	else if(j_Main.getDigital(ControllerDigital::right))
	{
		wantedFlywheelSpeed = flywheelManual ? 90 : 2800;
		triggerUpdate = true;
	}
	else if(j_Main.getDigital(ControllerDigital::down))
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


	if(j_Main.getDigital(ControllerDigital::X))
	{
		setFlywheelArmMode(true);
	}
	else
	{
		setFlywheelArmMode(false);
	}
}
