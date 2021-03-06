#include "ArmController.hpp"

ArmController::ArmController(Motor* iarm, IterativePosPIDController* ipid) :
  arm(iarm), pid(ipid), task(taskFnc, this) {
  calibrate();
}

void ArmController::calibrate() {
  arm->setBrakeMode(AbstractMotor::brakeMode::coast);
  arm->setEncoderUnits(AbstractMotor::encoderUnits::degrees);
  startAngle = arm->getPosition();
}

void ArmController::setState(armStates state) { armState = state; }

ArmController::armStates ArmController::getState() { return armState; }

double ArmController::getArmAngle() { return arm->getPosition() - startAngle; }

void ArmController::run() {
  double holdPos = 0;

  while (true) {

    switch (armState) {

      case off: arm->move(0); break;

      case hold:
        holdPos = getArmAngle();
        armState = holdAtPos;
        break;

      case holdAtPos:
        pid->setTarget(holdPos);
        arm->move(pid->step(getArmAngle()) * 127);
        break;

      case down:
        pid->setTarget(0);
        arm->move(pid->step(getArmAngle()) * 127);
        break;

      case carry:
        pid->setTarget(430);
        arm->move(pid->step(getArmAngle()) * 127);
        break;

      case balance:
        pid->setTarget(380);
        arm->move(pid->step(getArmAngle()) * 127);
        break;

      case descore:
        pid->setTarget(1130);
        arm->move(pid->step(getArmAngle()) * 127);
        break;

      case up: arm->move(127); break;

      case upSlow: arm->move(-70); break;
    }

    // std::cout << "Arm: " << getArmAngle() << std::endl;
    pros::delay(10);
  }
}

void ArmController::taskFnc(void* input) {
  pros::delay(500);
  ArmController* that = static_cast<ArmController*>(input);
  that->run();
}
