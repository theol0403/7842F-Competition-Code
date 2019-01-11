#include "DriverFunctions.hpp"

static intakeModes dIntakeState = intakeModes::off;
static intakeModes dLastIntakeState = intakeModes::off;

// r2 && r2 shoot everything
//r2 intake r1 just indake outtake
//l2 shoot l1 everything outtake


void driverIntakeControl()
{
	if(j_Digital(R2) && j_Digital(L2))
	{
		dIntakeState = intakeModes::shootBoth;
	}
	else if(j_Digital(R1) && j_Digital(L1))
	{
		dIntakeState = intakeModes::outSlow;
	}
	else if(j_Digital(R2))
	{
		dIntakeState = intakeModes::loading;
	}
	else if(j_Digital(L2))
	{
		dIntakeState = intakeModes::shootIndexer;
	}
	else if(j_Digital(L1))
	{
		dIntakeState = intakeModes::outIntake;
	}
	else if(j_Digital(R1))
	{
		dIntakeState = intakeModes::outBoth;
	}
	else
	{
		dIntakeState = intakeModes::off;
	}


	if(dIntakeState != dLastIntakeState)
	{
		switch (dIntakeState) {
			case intakeModes::off: {
				setIntakeMode(intakeModes::off);
				break;
			}
			case intakeModes::shootBoth: {
				setIntakeMode(intakeModes::shootBoth);
				break;
			}
			case intakeModes::loading: {
				setIntakeMode(intakeModes::loading);
				break;
			}
			case intakeModes::shootIndexer: {
				setIntakeMode(intakeModes::shootIndexer);
				break;
			}
			case intakeModes::outIntake: {
				setIntakeMode(intakeModes::outIntake);
				break;
			}
			case intakeModes::outBoth: {
				setIntakeMode(intakeModes::outBoth);
				break;
			}
			case intakeModes::outSlow: {
				setIntakeMode(intakeModes::outSlow);
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
