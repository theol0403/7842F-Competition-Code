#include "FlywheelController.hpp"

FlywheelController::FlywheelController(AbstractMotor* iflywheel, lib7842::velPID* ipid, double irpmEma, double iflywheelRatio, double islewRate) :
flywheel(iflywheel), pid(ipid), rpmFilter(irpmEma), flywheelRatio(iflywheelRatio), slewRate(islewRate),
flywheelTask(task, this) {}

void FlywheelController::setRPM(double rpm)
{
  targetRPM = rpm;
}

void FlywheelController::disable()
{
  disabled = true;
}

void FlywheelController::enable()
{
  disabled = false;
}

// calculate is on a scale of -1 - 1
// motorpower is on a scale of -1 to 1
// before motorpower was 127, so now it is scaled by 0.00787401574

void FlywheelController::run()
{
  while(true)
  {
    if(!disabled)
    {
      currentRPM = rpmFilter.filter(flywheel->getActualVelocity() * flywheelRatio);
      finalPower = pid->calculate(targetRPM, currentRPM);

      if(targetRPM <= 0) finalPower = 0;
      if(finalPower <= 0) finalPower = 0;
      if(finalPower > lastPower && lastPower < 10) lastPower = 10;
      if((finalPower - lastPower) > slewRate) finalPower = lastPower + slewRate;
      lastPower = finalPower;

      flywheel->moveVelocity(finalPower * 12000);
    }
    else
    {

    }

    pros::delay(10);
  }
}


void FlywheelController::task(void* input)
{
  FlywheelController* that = static_cast<FlywheelController*>(input);
  that->run();
}
