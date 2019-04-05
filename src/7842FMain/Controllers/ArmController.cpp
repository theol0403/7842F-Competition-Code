#include "ArmController.hpp"

ArmController::ArmController(Motor* iarm, IterativePosPIDController* ipid) :
arm(iarm), pid(ipid),
task(taskFnc, this)
{
  arm->setBrakeMode(AbstractMotor::brakeMode::hold);
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
	arm->tare_position();
  while(true)
  {


    switch(armState) {

      case off:

      break;

      case forward:
      arm->move_velocity(200);
      break;

      case back:
      arm->move_velocity(-200);
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
