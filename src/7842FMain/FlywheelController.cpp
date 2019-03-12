#include "FlywheelController.hpp"

FlywheelController::FlywheelController(IntakeController* iintake, Motor* iflywheel, ADIEncoder* isensor, VelMath* ivelMath, lib7842::velPID* ipid, double imotorSlew) :
intake(iintake), flywheel(iflywheel), sensor(isensor), velMath(ivelMath), pid(ipid), motorSlew(imotorSlew),
flywheelTask(task, this) {}

void FlywheelController::setRpm(double rpm)
{
  targetRPM = rpm;
}

double FlywheelController::getTargetRpm() {
  return targetRPM;
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

void FlywheelController::resetSlew()
{
  lastPower = 0;
  motorPower = 0;
}


void FlywheelController::run()
{
  while(true)
  {
    if(!disabled || intake->indexerSlave) //there is a motor available
    {
      currentRPM = velMath->step(sensor->get()).convert(rpm);

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
      motorPower = 0;
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
