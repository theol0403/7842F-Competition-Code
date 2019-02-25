#include "ArmController.hpp"

ArmController::ArmController(Motor* iarm, pros::ADIPotentiometer* iarmSensor, double ifoldAngle) :
arm(iarm), armSensor(iarmSensor), foldAngle(ifoldAngle)
armTask(task, this)
{
  arm->setBrakeMode(AbstractMotor::brakeMode::brake);
}

void ArmController::setState(armStates state) {
  armState = state;
}

double getArmAngle() {
  return (armSensor->get_value() / 4095.0 * 265.0) - foldAngle;
}


void ArmController::run()
{

  while(true)
  {

    switch(armState) {

      case armStates::off:
      intake->moveVelocity(0);
      indexerSlave = true;
      break;

      case armStates::intakeBall:
      intake->moveVelocity(200);
      if(hasBall) {
        indexerSlave = true;
      } else {
        indexerSlave = false;
        indexer->moveVelocity(80);
      }
      break;

      case armStates::outIntake:
      intake->moveVelocity(-200);
      indexerSlave = true;
      break;

    }

    pros::delay(10);
  }
}


void ArmController::task(void* input)
{
  ArmController* that = static_cast<ArmController*>(input);
  that->run();
}
