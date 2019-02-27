#include "IntakeController.hpp"

IntakeController::IntakeController(Motor* iintake, Motor* iindexer, pros::ADILineSensor* ilineSensor, double isensorEma) :
intake(iintake), indexer(iindexer), lineSensor(ilineSensor), sensorFilter(isensorEma),
intakeTask(task, this)
{
  intake->setBrakeMode(AbstractMotor::brakeMode::brake);
  //indexer->setBrakeMode(AbstractMotor::brakeMode::hold);
  lineSensor->calibrate();
}

void IntakeController::setState(intakeStates state) {
  intakeState = state;
}

IntakeController::intakeStates IntakeController::getState() {
  return intakeState;
}

void IntakeController::disable() {
  if(!disabled) {
    intake->moveVelocity(0);
    indexer->moveVelocity(0);
  }
  disabled = true;
}

void IntakeController::enable() {
  disabled = false;
}


void IntakeController::run()
{

  while(true)
  {
    double filteredSensor = sensorFilter.filter(lineSensor->get_value_calibrated());
    //std::cout << "Sensor: " << filteredSensor << std::endl;
    hasBall = filteredSensor < -80;

    if(!disabled)
    {
      switch(intakeState) {

        case off:
        intake->moveVelocity(0);
        indexerSlave = true;
        break;

        case intakeBall:
        intake->moveVelocity(200);
        if(hasBall) {
          indexerSlave = true;
        } else {
          indexerSlave = false;
          indexer->moveVelocity(80);
        }
        break;

        case outIntake:
        intake->moveVelocity(-200);
        indexerSlave = true;
        break;

        case outSlow:
        intake->moveVelocity(-50);
        indexerSlave = true;
        break;

      }

    }

    pros::delay(10);
  }
}


void IntakeController::task(void* input)
{
  IntakeController* that = static_cast<IntakeController*>(input);
  that->run();
}
