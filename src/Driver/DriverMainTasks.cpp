#include "main.h"
#include "Include/Shared/MotorConfig.hpp"
#include "Include/Driver/DriverMainTasks.hpp"





void DriverMainTask(void*)
{
	int joystickLeftX;
	int joystickRightY;
	int joystickRightX;

	bool flywheelManual = false;

	while(true)
	{

			joystickLeftX = j_Main.get_analog(ANALOG_LEFT_X);
			joystickRightY = j_Main.get_analog(ANALOG_RIGHT_Y);
			joystickRightX = j_Main.get_analog(ANALOG_RIGHT_X);

			setBasePower(joystickRightY + joystickLeftX, joystickRightY - joystickLeftX);
			setHPower(joystickRightX);


		if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
    {
      setIntakePower(127);
    }
    else if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    {
      setIntakePower(-127);
    }
    else
    {
			setIntakePower(0);
    }


		if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
    {
      setIndexerPower(-127);
    }
    else if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
      setIndexerPower(127);
    }
    else
    {
      setIndexerPower(0);
		}


    if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
    {
      wantedPower = 70;
    }
    else if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
    {
      wantedPower = 127;
    }
    else if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
    {
      wantedPower = 0;
    }



    if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_X))
    {
      m_flywheel.move(-127);
    }







		pros::delay(20);
	}

}
