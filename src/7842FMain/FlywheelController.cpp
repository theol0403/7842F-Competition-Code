#include "FlywheelController.hpp"

FlywheelController::FlywheelController(IntakeController* iintake, Motor* iflywheel, ADIEncoder* isensor, VelMath* ivelMath, EmaFilter* irpmFilter, lib7842::velPID* ipid, double imotorSlew) :
intake(iintake), flywheel(iflywheel), sensor(isensor), velMath(ivelMath), rpmFilter(irpmFilter), pid(ipid), motorSlew(imotorSlew),
flywheelTask(task, this) {
  sensor->reset();
}

void FlywheelController::setRpm(double rpm) { targetRPM = rpm; }

double FlywheelController::getTargetRpm() { return targetRPM; }

void FlywheelController::disable() {
  if(!disabled) flywheel->moveVoltage(0);
  disabled = true;
}

void FlywheelController::enable() {
  disabled = false;
}

void FlywheelController::resetSlew() {
  lastPower = 0;
  motorPower = 0;
}


void FlywheelController::run()
{
  jay::util::CSVwrite flywheelLogger("/ser/sout");
  flywheelLogger.WriteField("Time", false);
  flywheelLogger.WriteField("Target/4", false);
  flywheelLogger.WriteField("RPM/4", false);
  flywheelLogger.WriteField("Accel(m/s)", false);
  flywheelLogger.WriteField("Power", false);
  flywheelLogger.WriteField("D", true);

  while(true)
  {
    if(!disabled || intake->indexerSlave) //there is a motor available
    {
      currentRPM = rpmFilter->filter(velMath->step(sensor->get()).convert(rpm));

      motorPower = pid->calculate(targetRPM, currentRPM);

      if(motorPower <= 0) motorPower = 0; //Prevent motor from spinning backward
      //Give the motor a bit of a starting boost
      if(motorPower > lastPower && lastPower < 10 && motorPower > 10) lastPower = 10;

      //This slews the motor by limiting the rate of change of the motor speed
      double increment = motorPower - lastPower;
      if(std::abs(increment) > motorSlew) motorPower = lastPower + (motorSlew * lib7842::sgn(increment));
      lastPower = motorPower;

      //moves whatever motor is available
      if(!disabled) flywheel->move(motorPower);
      if(intake->indexerSlave) intake->indexer->move(-motorPower);
    }
    else //If no motors are available, approximate how much the flywheel slows down
    {
      //lastPower = lastPower <= 0 ? 0 : lastPower - 0.24;
      //motorPower = 0;
    }

    //std::cout << "Target: " << targetRPM << " RPM: " << currentRPM << " Power: "<< motorPower << " Error: "<< pid->getError() << "\n";
    flywheelLogger.WriteRecord({std::to_string(pros::millis()), std::to_string(targetRPM/4), std::to_string(currentRPM/4), std::to_string((velMath->getAccel() * 4_in * 1_pi).convert(meter / second)), std::to_string(motorPower), std::to_string(pid->getD())}, true);
    pros::delay(10);
  }
}



void FlywheelController::task(void* input)
{
  FlywheelController* that = static_cast<FlywheelController*>(input);
  that->run();
}
