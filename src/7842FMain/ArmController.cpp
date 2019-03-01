#include "ArmController.hpp"

ArmController::ArmController(Motor* iarm, IterativePosPIDController* ipid) :
arm(iarm), pid(ipid),
armTask(task, this)
{
  arm->setBrakeMode(AbstractMotor::brakeMode::coast);
  startAngle = arm->getPosition();
}

void ArmController::setState(armStates state) {
  armState = state;
}

ArmController::armStates ArmController::getState() {
  return armState;
}

double ArmController::getArmAngle() {
  return (arm->getPosition() / 4095.0 * 265.0) - startAngle;
}


void ArmController::run()
{

  while(true)
  {
    double downPos = 27;
    double outPos = 20;

    switch(armState) {

      case off:
      arm->move(0);
      break;

      case down:
      pid->setTarget(downPos);
      arm->move(pid->step(getArmAngle()) * 127);
      break;

      case out:
      pid->setTarget(outPos);
      arm->move(pid->step(getArmAngle()) * 127);
      break;

      case back:
      pid->setTarget(0);
      arm->move(pid->step(getArmAngle()) * 127);
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
