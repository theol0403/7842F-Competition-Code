#include "FlywheelController.hpp"

FlywheelController::FlywheelController(AbstractMotor* iflywheel, lib7842::velPID* ipid, double irpmEma, double iflywheelRatio, double islewRate) :
flywheel(iflywheel), pid(ipid), rpmFilter(irpmEma), flywheelRatio(iflywheelRatio), slewRate(islewRate)
flywheelTask(run, this)
{}

  FlywheelController::setRPM(double rpm)
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

      currentRPM = rpmFilter.filter(flywheel->getActualVelocity() * flywheelRatio);
      motorPower = flywheelPID.calculate(targetRPM, currentRPM);

      if(targetRPM <= 0) motorPower = 0;
      if(motorPower <= 0) motorPower = 0;
      if(motorPower > lastPower && lastPower < 10) lastPower = 10;
      if((motorPower - lastPower) > slewRate) motorPower = lastPower + slewRate;
      lastPower = motorPower;

      that->flywheel->moveVelocity(motorPower * 12000);


      pros::delay(10);
    }
  }
