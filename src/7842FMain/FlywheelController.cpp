#include "FlywheelController.hpp"

FlywheelController::FlywheelController(AbstractMotor* iflywheel, double iflywheelRatio, lib7842::velPID* ipid, double irpmEma, double irpmSlew, double imotorSlew) :
flywheel(iflywheel), flywheelRatio(iflywheelRatio), pid(ipid), rpmFilter(irpmEma), rpmSlew(irpmSlew), motorSlew(imotorSlew),
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
      std::cout << "currentRPM: " << currentRPM << std::endl;

      //if((targetRPM - slewRPM) > rpmSlew) targetRPM = slewRPM + rpmSlew;
      finalPower = pid->calculate(targetRPM, currentRPM);
      std::cout << "finalPower: " << finalPower << std::endl;

      if(finalPower <= 0) finalPower = 0; //stops from going negative
      //if(lastPower < 0.2 && finalPower > 0.2) lastPower = 0.2; //gives starting boost

      double increment = finalPower - lastPower;
      if(std::abs(increment) > motorSlew) finalPower = lastPower + (motorSlew * lib7842::sgn(increment));
      lastPower = finalPower;
      std::cout << "slewPower: " << finalPower << std::endl;

      flywheel->moveVelocity(finalPower * 12000);
    }

    pros::delay(100);
  }
}


void FlywheelController::task(void* input)
{
  FlywheelController* that = static_cast<FlywheelController*>(input);
  that->run();
}
