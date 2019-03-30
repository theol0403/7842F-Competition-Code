#include "IntakeController.hpp"

IntakeController::IntakeController(Motor* iintake, Motor* iindexer, pros::ADILineSensor* ilineSensor, double isensorEma) :
intake(iintake), indexer(iindexer), lineSensor(ilineSensor), sensorFilter(isensorEma),
task(taskFnc, this)
{
}

void IntakeController::setState(intakeStates state) {
  intakeState = state;
}

IntakeController::intakeStates IntakeController::getState() {
  return intakeState;
}

void IntakeController::disable() {
  if(!disabled) {
    intake->move(0);
    indexer->move(0);
  }
  disabled = true;
}

void IntakeController::enable() {
  disabled = false;
}


void IntakeController::run()
{

  intake->setBrakeMode(AbstractMotor::brakeMode::brake);
  lineSensor->calibrate();

  while(true)
  {
    double filteredSensor = sensorFilter.filter(lineSensor->get_value_calibrated());
    //std::cout << "Sensor: " << filteredSensor << std::endl;
    hasBall = filteredSensor < -100;

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
          indexer->moveVelocity(60);
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

        case shoot:
        intake->moveVelocity(200);
        indexer->moveVelocity(50);
        indexerSlave = false;
        break;

      }
    }
    pros::delay(10);
  }
}


void IntakeController::taskFnc(void* input)
{
  pros::delay(500);
  IntakeController* that = static_cast<IntakeController*>(input);
  that->run();
}
