#include "main.h"
#include "Include/Shared/FlywheelTask.hpp"
#include "Include/Shared/MotorConfig.hpp"
#include "Include/Shared/PIDTuner.hpp"

int wantedFlywheelRPM = 0;

void setFlywheelRPM(int wantedRPM)
{
  wantedFlywheelRPM = wantedRPM;
}


double countMultiplier = 0.1;


void flywheelTask(void*)
{


  pidTune_t flywheelPIDParams = {0.23, 0, 0.1, 0.1, 0.8, 0.8};

  PIDScreenTuner tuneFlywheel(&flywheelPIDParams, LV_HOR_RES, LV_VER_RES);

  tunerButtons_t buttonKp;
  tuneFlywheel.initButton(&buttonKp, 50, &flywheelPIDParams.kP, "kP");
  tunerButtons_t buttonKd;
  tuneFlywheel.initButton(&buttonKd, 150, &flywheelPIDParams.kD, "kD");
  tunerButtons_t buttonKf;
  tuneFlywheel.initButton(&buttonKf, 250, &flywheelPIDParams.kF, "kF");

  tunerButtons_t buttonMultiplier;
  tuneFlywheel.initButton(&buttonMultiplier, 350, &countMultiplier, "Multiplier", true);



auto FlywheelRPM = VelMathArgs(imev5TPR * 15, std::make_shared<EmaFilter>(0.5));
auto FlywheelPID = IterativeControllerFactory::velPID(0.01, 1, 0.1, 0, FlywheelRPM, std::make_unique<EmaFilter>(0.04));
FlywheelPID.setOutputLimits(127, -127);


    const int slewRate = 1;

      int motorPower = 0;
    	int lastPower = 0;

    	while(true)
    	{
        FlywheelPID.setTarget(wantedFlywheelRPM);

        motorPower = FlywheelPID.step(getFlywheelEncoder());

    		if(motorPower < 0) motorPower = 0;
    		if(motorPower > lastPower && lastPower < 20) lastPower = 20;
    		if((motorPower - lastPower) > slewRate) motorPower = lastPower + slewRate;
        lastPower = motorPower;

    		setFlywheelPower(motorPower);

        std::cout << "RPM: " << FlywheelPID.getVel().convert(rpm) << " Power: "<< motorPower << " Error: "<< FlywheelPID.getError() << "\n";


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
