#include "DriverFunctions.hpp"

enum dIntakeStates_t
{
	none,
	both,
	bottom,
	top,
	outButton
};

static dIntakeStates_t dIntakeState = none;
static dIntakeStates_t dLastIntakeState = none;

void driverIntakeControl()
{
	if(j_Digital(R1) && j_Digital(R2))
	{
		dIntakeState = both;
	}
	else if(j_Digital(R1))
	{
		dIntakeState = top;
	}
	else if(j_Digital(R2))
	{
		dIntakeState = bottom;
	}
	else
	{
		dIntakeState = none;
	}

	if(j_Digital(Y))
	{
		dIntakeState = outButton;
	}
	

	if(dIntakeState != dLastIntakeState)
	{
		switch (dIntakeState) {
			case none: {
				setIntakeMode(intakeModes::off);
				break;
			}
			case both: {
				setIntakeMode(intakeModes::shootBoth);
				break;
			}
			case bottom: {
				setIntakeMode(intakeModes::loading);
				break;
			}
			case top: {
				setIntakeMode(intakeModes::shootIndexer);
				break;
			}
			case outButton: {
				setIntakeMode(intakeModes::out);
				break;
			}

		}

		dLastIntakeState = dIntakeState;
	}

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
