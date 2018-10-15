#include "main.h"
#include "MotorConfig.hpp"

int joystickCh4;
int joystickCh2;

const int driverBaseThreshold = 15;


int armMaxLimit = 3800;
int armMinLimit = 890;


void DriverMainTask(void*)
{


	while(true)
	{

		// Base Control --------------------------------------------------------------------------------


			joystickCh4 = j_Main.get_analog(ANALOG_LEFT_X);
			joystickCh2 = j_Main.get_analog(ANALOG_RIGHT_Y);

			joystickCh4 = abs(joystickCh4) > driverBaseThreshold ? joystickCh4 : 0;
			joystickCh2 = abs(joystickCh2) > driverBaseThreshold ? joystickCh2 : 0;


			setBasePower(joystickCh2 + joystickCh4, joystickCh2 - joystickCh4);
		// Base Control --------------------------------------------------------------------------------




		// Intake --------------------------------------------------------------------------------
		if(vexRT[Btn6U])
		{
			setIntakePower(-70);
			setIndexerPower(-127);
		}
		else if(vexRT[Btn6D])
		{
			setIntakePower(127);
			setIndexerPower(15);
		}
		else
		{
			setIntakePower(0);
			setIndexerPower(0);
		}

		if(vexRT[Btn5D])
		{
			setIndexerPower(127);
		}
		else if(vexRT[Btn5U])
		{
			setIndexerPower(-127);
		}

		// Intake --------------------------------------------------------------------------------

		// Index --------------------------------------------------------------------------------


		// Index --------------------------------------------------------------------------------

		if(flywheelManual)
		{
			if(vexRT[Btn7D])
			{
				setFlywheelPower(0);
			}
			else if(vexRT[Btn7L])
			{
				setFlywheelPower(70);
			}
			else if(vexRT[Btn7U])
			{
				setFlywheelPower(90);
			}
			else if(vexRT[Btn7R])
			{
				setFlywheelPower(110);
			}
		}
		else
		{
			if(vexRT[Btn7D])
			{
				wantedFlywheelRPM = 0;
			}
			else if(vexRT[Btn7L])
			{
				wantedFlywheelRPM = 2300;
			}
			else if(vexRT[Btn7U])
			{
				wantedFlywheelRPM = 2500;
			}
			else if(vexRT[Btn7R])
			{
				wantedFlywheelRPM = 2800;
			}
		}




		if(vexRT[Btn8R] && !flywheelManual)
		{
			stopTask(FlywheelPIDTask);
			flywheelManual = true;
		}
		else if(vexRT[Btn8L] && flywheelManual)
		{
			startTask(FlywheelPIDTask);
			flywheelManual = false;
		}




		if(vexRT[Btn8U] && SensorValue[s_armPot] > armMinLimit + 500)
		{
			setArmPower(100);
		}
		else if(vexRT[Btn8D] && SensorValue[s_armPot] < armMaxLimit)
		{
			setArmPower(-100);
		}
		else
		{
			setArmPower(0);
		}

		if(SensorValue[s_armPot] < armMinLimit)
		{
			setArmPower(-50);
		}
		else if(SensorValue[s_armPot] > armMaxLimit)
		{
			setArmPower(50);
		}






		wait1Msec(20);
	}

}
