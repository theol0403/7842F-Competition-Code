#include "main.h"
#include "Include/Shared/FlywheelTask.hpp"
#include "Include/Shared/MotorConfig.hpp"




void flywheelTask(void*)
{


auto FlywheelRPM = VelMathArgs(imev5TPR * 15, std::make_shared<EmaFilter>(0.5));
auto FlywheelPID = IterativeControllerFactory::velPID(0.01, 1, 0.1, 0, FlywheelRPM, std::make_unique<EmaFilter>(0.04));
FlywheelPID.setOutputLimits(127, -127);

FlywheelPID.setTarget(2000);


    const int slewRate = 1;


      int motorPower = 0;
    	int lastPower = 0;

      QAngularSpeed flywheelRPM = 0_rpm;


    	while(true)
    	{

        motorPower = FlywheelPID.step(getFlywheelEncoder());

        flywheelRPM = FlywheelPID.getVel();



    		if(motorPower < 0) motorPower = 0;

    		if(motorPower > lastPower && lastPower < 20) lastPower = 20;

    		if((motorPower - lastPower) > slewRate) motorPower = lastPower + slewRate;
        lastPower = motorPower;



    		setFlywheelPower(motorPower);

        std::cout << "RPM: " << (rpm) flywheelRPM << " Power: "<< motorPower << "\n";


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
