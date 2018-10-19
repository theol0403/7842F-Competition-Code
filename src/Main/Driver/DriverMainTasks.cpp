#include "main.h"
#include "Include/Shared/MotorConfig.hpp"
#include "Include/Driver/DriverMainTasks.hpp"

int joystickLeftX;
int joystickRightY;
int joystickRightX;

const int BASE_THRESHOLD = 0;



void DriverMainTask(void*)
{

	while(true)
	{

		// Base Control --------------------------------------------------------------------------------


			joystickLeftX = j_Main.get_analog(ANALOG_LEFT_X);
			joystickRightY = j_Main.get_analog(ANALOG_RIGHT_Y);
			joystickRightX = j_Main.get_analog(ANALOG_RIGHT_X);

			joystickLeftX = abs(joystickLeftX) > BASE_THRESHOLD ? joystickLeftX : 0;
			joystickRightY = abs(joystickRightY) > BASE_THRESHOLD ? joystickRightY : 0;
			joystickRightX = abs(joystickRightX) > BASE_THRESHOLD ? joystickRightX : 0;


			setBasePower(joystickRightY + joystickLeftX, joystickRightY - joystickLeftX);
			setHPower(joystickRightX);
		// Base Control --------------------------------------------------------------------------------



		pros::delay(20);
	}

}
