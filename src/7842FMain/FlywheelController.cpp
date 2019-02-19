#include "FlywheelController.hpp"

FlywheelController::FlywheelController(AbstractMotor* iflywheel, double iflywheelRatio, lib7842::velPID* ipid) :
flywheel(iflywheel), flywheelRatio(iflywheelRatio), pid(ipid),
rpmFilter(1), rpmSlew(3000), motorSlew(0.35),
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
    if(disabled)
    {
      lastPower = lastPower <= 0 ? 0 : lastPower - 0.27;
    }
    else
    {
      currentRPM = rpmFilter.filter(flywheel->getActualVelocity() * flywheelRatio);
      motorPower = pid->calculate(targetRPM, currentRPM);

      if(targetRPM <= 0) motorPower = 0;
      if(motorPower <= 0) motorPower = 0;
      if(motorPower > lastPower && lastPower < 10) lastPower = 10;
      if((motorPower - lastPower) > motorSlew) motorPower = lastPower + motorSlew;
      lastPower = motorPower;

      flywheel->moveVoltage(motorPower/127.0*12000);
    }

    //std::cout << "RPM: " << currentRPM << " Power: "<< motorPower << " Error: "<< flywheelPID.getError() << "\n";

    pros::delay(10);
  }
}



void FlywheelController::task(void* input)
{
  FlywheelController* that = static_cast<FlywheelController*>(input);
  that->run();
}
