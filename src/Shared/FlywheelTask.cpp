#include "main.h"
#include "Include/Shared/FlywheelTask.hpp"
#include "Include/Shared/MotorConfig.hpp"




void flywheelTask(void*)
{

  pros::Motor m_flywheel(15, true);
  pros::Motor m_intake(14);

  float slewRate = 1;
  float motorPower = 0;
  float lastPower = 0;


  while(true)
  {




    if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
    {
      m_flywheel.move(-127);
    }
    else if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    {
      motorPower = 127;
      if(motorPower > lastPower && lastPower < 20) lastPower = 20;
      if((motorPower - lastPower) > slewRate) motorPower = lastPower + slewRate;
      m_flywheel.move(motorPower);
    }
    else
    {
      motorPower = 0;
      m_flywheel.move(0);
    }

    lastPower = motorPower;




    if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
    {
      m_intake.move(-127);
    }
    else if(j_Main.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
      m_intake.move(127);
    }
    else
    {
      m_intake.move(0);
    }


    pros::delay(20);
  }
}








// void flywheelTask(void*)
// {
//
//   pros::Motor m_flywheel(15, true);
//
//
// auto flywheelPID = IterativeControllerFactory::velPID(0.01, 1, 0.1);
//
// 	int slewRate = 1;
//   float motorPower = 0;
// 	float lastPower = 0;
//
// flywheelPID.setTarget(5);
//
// 	while(true)
// 	{
//
//
//     motorPower = flywheelPID.step(m_flywheel.get_actual_velocity()) * 127;
//
// std::cout << motorPower;
//
// 		if(motorPower < 0) motorPower = 0;
//
// 		if(motorPower > lastPower && lastPower < 20) lastPower = 20;
//
// 		if((motorPower - lastPower) > slewRate) motorPower = lastPower + slewRate;
//     lastPower = motorPower;
//
//
// std::cout << motorPower;
//
// 	m_flywheel.move(motorPower);
//
//
//
// 		pros::delay(50);
// 	}
// }
