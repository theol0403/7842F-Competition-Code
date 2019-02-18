#include "FlywheelController.hpp"

FlywheelController::FlywheelController(AbstractMotor* iflywheel, lib7842::velPID* ipid, double irpmEma, double iflywheelRatio, double islewRate) :
flywheel(iflywheel), pid(ipid), rpmFilter(irpmEma), flywheelRatio(iflywheelRatio), slewRate(islewRate),
flywheelTask(run, this)
{}

  void FlywheelController::setRPM(double rpm)
  {
    targetRPM = rpm;
  }

  // calculate is on a scale of -1 - 1
  // motorpower is on a scale of -1 to 1
  // before motorpower was 127, so now it is scaled by 0.00787401574

  void FlywheelController::run(void* input)
  {
    FlywheelController* that = static_cast<FlywheelController*>(input);

    while(true)
    {

      that->currentRPM = that->rpmFilter.filter(that->flywheel->getActualVelocity() * that->flywheelRatio);
      that->finalPower = that->pid->calculate(that->targetRPM, that->currentRPM);

      if(that->targetRPM <= 0) that->finalPower = 0;
      if(that->finalPower <= 0) that->finalPower = 0;
      if(that->finalPower > that->lastPower && that->lastPower < 10) that->lastPower = 10;
      if((that->finalPower - that->lastPower) > that->slewRate) that->finalPower = that->lastPower + that->slewRate;
      that->lastPower = that->finalPower;

      that->flywheel->moveVelocity(that->finalPower * 12000);


      pros::delay(10);
    }
  }
