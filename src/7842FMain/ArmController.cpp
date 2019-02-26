#include "ArmController.hpp"

ArmController::ArmController(Motor* iarm, pros::ADIPotentiometer* iarmSensor, double ifoldAngle, IterativePosPIDController* ipid) :
arm(iarm), armSensor(iarmSensor), foldAngle(ifoldAngle), pid(ipid),
armTask(task, this)
{
  arm->setBrakeMode(AbstractMotor::brakeMode::coast);
}

void ArmController::setState(armStates state) {
  armState = state;
}

double ArmController::getArmAngle() {
  return (armSensor->get_value() / 4095.0 * 265.0) - foldAngle;
}


void ArmController::run()
{

  while(true)
  {
    double outPos = 117;
    double downPos = 170;
    double upPos = 68;

    switch(armState) {

      case off:
      arm->move(0);
      break;

      case unfold:
      arm->move(127);
      if(getArmAngle() > outPos) armState = out;
      break;

      case out:
      pid->setTarget(outPos);
      arm->move(pid->step(getArmAngle()) * 127);
      break;

      case down:
      pid->setTarget(downPos);
      arm->move(pid->step(getArmAngle()) * 127);
      break;

      case up:
      pid->setTarget(upPos);
      double motorPower = pid->step(getArmAngle());
      if(motorPower < 0) arm->move(motorPower * 127); //does not allow arm to fall down but can be picked up
      else arm->move(0);
      break;

    }

    //std::cout << "Arm: " << getArmAngle() << std::endl;
    pros::delay(10);
  }
}


void ArmController::task(void* input)
{
  ArmController* that = static_cast<ArmController*>(input);
  that->run();
}
