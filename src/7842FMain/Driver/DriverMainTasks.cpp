#include "DriverMainTasks.hpp"


void DriverMainTask(void*)
{



	int wantedFlywheelSpeed = 0;
	bool triggerUpdate = true;
	bool flywheelManual = false;
	bool manualToggle = false;

	while(true)
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


		if(j_Main.getDigital(ControllerDigital::Y) && !manualToggle)
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
		else if(!j_Main.getDigital(ControllerDigital::Y) && manualToggle)
		{
			manualToggle = false;
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


		if(j_Main.getDigital(ControllerDigital::X))
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
