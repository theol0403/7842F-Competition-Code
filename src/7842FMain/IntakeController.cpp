#include "IntakeController.hpp"

IntakeController::IntakeController(AbstractMotor* iintake, AbstractMotor* iindexer, pros::ADILineSensor* ilineSensor, double isensorEma) :
intake(iintake), indexer(iindexer), lineSensor(ilineSensor), sensorFilter(isensorEma),
intakeTask(run, this)
{
  intake->setBrakeMode(AbstractMotor::brakeMode::hold);
  indexer->setBrakeMode(AbstractMotor::brakeMode::hold);
  lineSensor->calibrate();
}

void IntakeController::setState(intakeStates state)
{
  intakeState = state;
}

void IntakeController::run(void* input)
{
  IntakeController* that = static_cast<IntakeController*>(input);

  while(true)
  {
    double filteredSensor = that->sensorFilter.filter(that->lineSensor->get_value_calibrated());

    switch(that->intakeState)
    {

      case intakeStates::off:
      {
        that->intake->moveVelocity(0);
        that->indexer->moveVelocity(0);
        break;
      }

      case intakeStates::loading:
      {
        //Intake and collect until ball is in sensor
        //once ball is in sensor index brake
        //collecting
        if(filteredSensor < -200)
        {
          that->intakeState = intakeStates::collecting;
        }
        else
        {
          that->intake->moveVelocity(200);
          that->indexer->moveVelocity(100);
        }
        break;
      }

      case intakeStates::collecting:
      {
        //Run intake
        that->intake->moveVelocity(200);
        that->indexer->moveVelocity(0);
        break;
      }

      case intakeStates::shootIndexer:
      {
        that->intake->moveVelocity(0);
        that->indexer->moveVelocity(200);
        that->intakeState = intakeStates::loading; // Allows mode to be set to off while waiting
        pros::delay(200);
        break;
      }

      case intakeStates::shootBoth:
      {
        that->intake->moveVelocity(200);
        that->indexer->moveVelocity(200);
        //intakeState = intakeStates::loading; // Allows mode to be set to off while waiting
        pros::delay(200);
        break;
      }

      case intakeStates::outIntake:
      {
        that->intake->moveVelocity(-200);
        that->indexer->moveVelocity(0);
        break;
      }

      case intakeStates::outBoth:
      {
        that->intake->moveVelocity(-200);
        that->indexer->moveVelocity(-100);
        break;
      }

      case intakeStates::outSlow:
      {
        that->intake->moveVelocity(-40);
        that->indexer->moveVelocity(-40);
        break;
      }

      case intakeStates::compress:
      {
        that->intake->moveVelocity(80);
        that->indexer->moveVelocity(-40);
        break;
      }

    }

    pros::delay(10);
  }

}
