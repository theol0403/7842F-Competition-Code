#include "main.h"
#include "Include/Shared/FlywheelTask.hpp"


double wantedFlywheelRPM = 0;
bool extendArm = false;


void flywheelTask(void*)
{

  int flywheelRPM = 0;

  const double slewRate = 0.7;
  double lastPower = 0;
  double motorPower = 0;


  lib7842::velPID flywheelPID(0.4, 0.05, 0.044, 0.9);
  lib7842::emaFilter rpmEma(0.15);

  while(true)
  {
    setFlywheelPower(0);
    while(wantedFlywheelRPM == 0 && !extendArm) {pros::delay(20);} //Wait until power > 0
    lastPower = getFlywheelRPM() / 3000 * 127; //Hopefully power should resume to motor

    while(wantedFlywheelRPM || extendArm) //Loop until power is back to 0
    {
      if(extendArm)
      {
        setFlywheelPower(-80);
        lastPower = lastPower <= 0 ? 0 : lastPower-0.27;
      }
      else
      {
        flywheelRPM = rpmEma.filter(getFlywheelRPM());
        motorPower = flywheelPID.calculate(wantedFlywheelRPM, flywheelRPM);

        if(wantedFlywheelRPM <= 0) motorPower = 0;
        if(motorPower <= 0) motorPower = 0;
        if(motorPower > lastPower && lastPower < 10) lastPower = 10;
        if((motorPower - lastPower) > slewRate) motorPower = lastPower + slewRate;
        lastPower = motorPower;

        setFlywheelPower(motorPower);
      }


      //std::cout << "RPM: " << flywheelRPM << " Power: "<< motorPower << " Error: "<< flywheelPID.getError() << "\n";

      pros::delay(20);
    }
  }
}



void setFlywheelRPM(int wantedRPM)
{
  wantedFlywheelRPM = wantedRPM;
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
