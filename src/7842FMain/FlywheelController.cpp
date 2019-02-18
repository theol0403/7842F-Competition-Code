#include "FlywheelController.hpp"

FlywheelController::FlywheelController(AbstractMotor* iflywheel, lib7842::velPID* ipid, double irpmEma) :
flywheel(iflywheel), pid(ipid), rpmFilter(irpmEma)
flywheelTask(run, this)
{}

FlywheelController::setRPM(double rpm)
{
  targetRPM = rpm;
}

// calculate is on a scale of -1 - 1
// motorpower is on a scale of 12000
// before motorpower was 127, so now it is scaled by 94.4881889764

void FlywheelController::run(void* input)
{
  FlywheelController* that = static_cast<FlywheelController*>(input);

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

      pros::delay(10);
    }
  }

}
