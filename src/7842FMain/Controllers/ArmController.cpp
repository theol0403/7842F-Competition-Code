#include "ArmController.hpp"

ArmController::ArmController(Motor* iarm, IterativePosPIDController* ipid) :
arm(iarm), pid(ipid),
task(taskFnc, this)
{
  arm->setBrakeMode(AbstractMotor::brakeMode::coast);
  arm->setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  startAngle = arm->getPosition();
}

void ArmController::setState(armStates state) {
  armState = state;
}

ArmController::armStates ArmController::getState() {
  return armState;
}

double ArmController::getArmAngle() {
  return arm->getPosition() - startAngle;
}


void ArmController::run()
{
  double holdPos = 0;

  while(true)
  {

    switch(armState) {

      case off:
      arm->move(0);
      break;

      case hold:
      holdPos = getArmAngle();
      armState = holdAtPos;
      break;

      case holdAtPos:
      pid->setTarget(holdPos);
      arm->move(pid->step(getArmAngle()) * 127);
      break;

      case down:
      pid->setTarget(20);
      arm->move(pid->step(getArmAngle()) * 127);
      break;

      case carry:
      pid->setTarget(340);
      arm->move(pid->step(getArmAngle()) * 127);
      break;

      case balance:
      pid->setTarget(380);
      arm->move(pid->step(getArmAngle()) * 127);
      break;

      case on:
      arm->move(127);
      break;
    }

    //std::cout << "Arm: " << getArmAngle() << std::endl;
    pros::delay(10);
  }
}


void ArmController::taskFnc(void* input)
{
  pros::delay(500);
  ArmController* that = static_cast<ArmController*>(input);
  that->run();
}
