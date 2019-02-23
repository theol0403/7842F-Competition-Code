#include "IntakeController.hpp"

IntakeController::IntakeController(AbstractMotor* iintake, AbstractMotor* iindexer, pros::ADILineSensor* ilineSensor, double isensorEma, FlywheelController* iflywheel) :
intake(iintake), indexer(iindexer), lineSensor(ilineSensor), sensorFilter(isensorEma), flywheel(iflywheel),
intakeTask(task, this)
{
  intake->setBrakeMode(AbstractMotor::brakeMode::hold);
  indexer->setBrakeMode(AbstractMotor::brakeMode::hold);
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

void IntakeController::moveIndexerSlave() {
  indexer->move(-flywheel->motorPower);
}

void IntakeController::run()
{

  while(true)
  {
    double filteredSensor = sensorFilter.filter(lineSensor->get_value_calibrated());

    if(!disabled)
    {
      switch(intakeState) {

        case intakeStates::off:
        intake->moveVelocity(0);
        moveIndexerSlave();
        break;

        case intakeStates::loading:
        //Intake and collect until ball is in sensor
        //once ball is in sensor index brake
        //collecting
        if(filteredSensor < -200) {
          intakeState = intakeStates::collecting;
        } else {
          intake->moveVelocity(200);
          indexer->moveVelocity(100);
        }
        break;

        case intakeStates::collecting:
        //Run intake
        intake->moveVelocity(200);
        moveIndexerSlave();
        break;


        case intakeStates::outIntake:
        intake->moveVelocity(-200);
        moveIndexerSlave();
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
