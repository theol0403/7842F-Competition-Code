#include "FlywheelController.hpp"

FlywheelController::FlywheelController(AbstractMotor* iflywheel, lib7842::velPID* ipid, double irpmEma, double iflywheelRatio, double irpmSlew, double imotorSlew) :
flywheel(iflywheel), pid(ipid), rpmFilter(irpmEma), flywheelRatio(iflywheelRatio), rpmSlew(irpmSlew), motorSlew(imotorSlew),
flywheelTask(task, this) {}

void FlywheelController::setRpm(double rpm)
{
  targetRPM = rpm;
}

void FlywheelController::disable()
{
  disabled = true;
  flywheel->moveVelocity(0);
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

      if((targetRPM - slewRPM) > rpmSlew) targetRPM = slewRPM + rpmSlew;
      finalPower = pid->calculate(targetRPM, currentRPM);

      if(finalPower <= 0) finalPower = 0; //stops from going negative
      if(finalPower > lastPower && lastPower < 0.2) lastPower = 0.2; //gives starting boost
      double increment = finalPower - lastPower;
      if(std::abs(increment) > motorSlew) finalPower = lastPower + (motorSlew * lib7842::sgn(increment));
      lastPower = finalPower;

      flywheel->moveVelocity(finalPower * 12000);
    }

    pros::delay(10);
  }
}


void FlywheelController::task(void* input)
{
  FlywheelController* that = static_cast<FlywheelController*>(input);
  that->run();
}
