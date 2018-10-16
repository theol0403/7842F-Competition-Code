#include "main.h"
#include "MotorConfig.hpp"
#include "DriverMainTasks.hpp"

int joystickCh4;
int joystickCh2;

const int BASE_THRESHOLD = 15;



void DriverMainTask(void*)
{

	while(true)
	{

		// Base Control --------------------------------------------------------------------------------


			joystickCh4 = j_Main.get_analog(ANALOG_LEFT_X);
			joystickCh2 = j_Main.get_analog(ANALOG_RIGHT_Y);

			joystickCh4 = abs(joystickCh4) > BASE_THRESHOLD ? joystickCh4 : 0;
			joystickCh2 = abs(joystickCh2) > BASE_THRESHOLD ? joystickCh2 : 0;


			setBasePower(joystickCh2 + joystickCh4, joystickCh2 - joystickCh4);
		// Base Control --------------------------------------------------------------------------------



		pros::delay(20);
	}

}
