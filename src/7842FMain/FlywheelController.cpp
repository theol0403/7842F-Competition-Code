#include "FlywheelController.hpp"

FlywheelController::FlywheelController(Motor* iflywheel, double iflywheelRatio, lib7842::velPID* ipid, lib7842::emaFilter* irpmFilter, double imotorSlew) :
flywheel(iflywheel), flywheelRatio(iflywheelRatio), pid(ipid),
rpmFilter(irpmFilter), motorSlew(imotorSlew),
flywheelTask(task, this) {}

void FlywheelController::setRpm(double rpm)
{
  targetRPM = rpm;
}

void FlywheelController::disable()
{
  if(!disabled) flywheel->moveVoltage(0);
  disabled = true;
}

void FlywheelController::enable()
{
  disabled = false;
}


void FlywheelController::run()
{
  while(true)
  {
    if(disabled)
    {
      lastPower = lastPower <= 0 ? 0 : lastPower - 0.25; //tune, was 0.27
      motorPower = 0;
    }
    else
    {
      currentRPM = rpmFilter->filter(flywheel->getActualVelocity() * flywheelRatio);
      motorPower = pid->calculate(targetRPM, currentRPM);

      if(motorPower <= 0) motorPower = 0;
      if(motorPower > lastPower && lastPower < 10 && motorPower > 10) lastPower = 10;

      double increment = motorPower - lastPower;
      if(std::abs(increment) > motorSlew) motorPower = lastPower + (motorSlew * lib7842::sgn(increment));
      lastPower = motorPower;

      if(!disabled) flywheel->move(motorPower);
    }

    //std::cout << "RPM: " << currentRPM << " Power: "<< motorPower << " Error: "<< flywheelPID.getError() << "\n";

    pros::delay(20);
  }
}



void FlywheelController::task(void* input)
{
  FlywheelController* that = static_cast<FlywheelController*>(input);
  that->run();
}
