#include "DriverMainTasks.hpp"


void DriverMainTask(void*)
{

	int joystickLeftX;
	int joystickRightY;
	int joystickRightX;

	int wantedFlywheelSpeed = 0;
	bool triggerUpdate = true;
	bool flywheelManual = false;
	bool manualToggle = false;

	while(true)
	{

		joystickLeftX = j_Main.get_analog(ANALOG_LEFT_X);
		joystickRightY = j_Main.get_analog(ANALOG_RIGHT_Y);
		joystickRightX = j_Main.get_analog(ANALOG_RIGHT_X);

		setBasePower(joystickRightY, joystickRightX, joystickLeftX);


		if(j_Main.get_digital(DIGITAL_R1))
		{
			setIntakePower(90);
		}
		else if(j_Main.get_digital(DIGITAL_R2))
		{
			setIntakePower(-127);
		}
		else
		{
			setIntakePower(0);
		}


		if(j_Main.get_digital(DIGITAL_L1))
		{
			setIndexerPower(127);
		}
		else if(j_Main.get_digital(DIGITAL_L2))
		{
			setIndexerPower(-127);
		}
		else
		{
			setIndexerPower(0);
		}


		if(j_Main.get_digital(DIGITAL_Y) && !manualToggle)
		{
			manualToggle = true;
			if(flywheelManual)
			{
				flywheelManual = false;
			}
			else
			{
				flywheelManual = true;
			}
		}
		else if(!j_Main.get_digital(DIGITAL_Y) && manualToggle)
		{
			manualToggle = false;
		}


		if(j_Main.get_digital(DIGITAL_LEFT))
		{
			wantedFlywheelSpeed = flywheelManual ? 50 : 2400;
			triggerUpdate = true;
		}
		else if(j_Main.get_digital(DIGITAL_UP))
		{
			wantedFlywheelSpeed = flywheelManual ? 70 : 2600;
			triggerUpdate = true;
		}
		else if(j_Main.get_digital(DIGITAL_RIGHT))
		{
			wantedFlywheelSpeed = flywheelManual ? 90 : 2800;
			triggerUpdate = true;
		}
		else if(j_Main.get_digital(DIGITAL_DOWN))
		{
			wantedFlywheelSpeed = 0;
			triggerUpdate = true;
		}

		if(triggerUpdate)
		{
			if(flywheelManual)
			{
				setFlywheelPower(wantedFlywheelSpeed);
				setFlywheelRPM(0);
			}
			else
			{
				setFlywheelRPM(wantedFlywheelSpeed);
			}
			triggerUpdate = false;
		}


if(j_Main.get_digital(DIGITAL_X))
{
	extendArm = true;
}
else
{
	extendArm = false;
}



		pros::delay(30);
	}

}
