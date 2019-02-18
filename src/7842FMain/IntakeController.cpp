#include "IntakeController.hpp"

IntakeController::IntakeController(AbstractMotor* iintake, AbstractMotor* iindexer, pros::ADILineSensor* ilineSensor, double isensorEma) :
intake(iintake), indexer(iindexer), lineSensor(ilineSensor), sensorFilter(isensorEma),
intakeTask(run, this)
{}

  IntakeController::setState(intakeStates state)
  {
    intakeState = state;
  }

  void run(void* input)
  {
    IntakeController* that = static_cast<IntakeController*>(input);

    while(true)
    {
      double filteredSensor = sensorFilter.filter(getIndexerSensor());
      // std::cout << "Sensor:" << getIndexerSensor() << std::endl;
      // std::cout << "Filter:" << filteredSensor << std::endl;

      switch(intakeMode)
      {

        case intakeModes::off:
        {
          setIntakeVelocity(0);
          setIndexerVelocity(0);
          break;
        }

        case intakeModes::loading:
        {
          //Intake and collect until ball is in sensor
          //once ball is in sensor index brake
          //collecting
          if(filteredSensor < -200)
          {
            intakeMode = intakeModes::collecting;
          }
          else
          {
            setIntakeVelocity(200);
            setIndexerVelocity(100);
          }
          break;
        }

        case intakeModes::collecting:
        {
          //Run intake
          setIntakeVelocity(200);
          setIndexerVelocity(0);
          break;
        }

        case intakeModes::shootIndexer:
        {
          setIntakeVelocity(0);
          setIndexerVelocity(200);
          intakeMode = intakeModes::loading; // Allows mode to be set to off while waiting
          pros::delay(200);
          break;
        }

        case intakeModes::shootBoth:
        {
          setIntakeVelocity(200);
          setIndexerVelocity(200);
          //intakeMode = intakeModes::loading; // Allows mode to be set to off while waiting
          pros::delay(200);
          break;
        }

        case intakeModes::outIntake:
        {
          setIntakeVelocity(-200);
          setIndexerVelocity(0);
          break;
        }

        case intakeModes::outBoth:
        {
          setIntakeVelocity(-200);
          setIndexerVelocity(-100);
          break;
        }

        case intakeModes::outSlow:
        {
          setIntakeVelocity(-50);
          setIndexerVelocity(-50);
          break;
        }

      }

      pros::delay(20);
    }
