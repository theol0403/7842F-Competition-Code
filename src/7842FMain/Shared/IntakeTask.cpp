#include "IntakeTask.hpp"

static intakeModes intakeMode = intakeModes::off;
void setIntakeMode(intakeModes iIntakeMode)
{
  intakeMode = iIntakeMode;
}

void intakeControlTask(void*)
{
  okapi::EmaFilter intakeFilter(0.6);
  while(true)
  {
    double filteredSensor = intakeFilter.filter(getIndexerSensor());
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
        setIntakeVelocity(200);
        setIndexerVelocity(100);
        if(filteredSensor < -360)
        {
          intakeMode = intakeModes::collecting;
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

      case intakeModes::shootBoth:
      {
        setIntakeVelocity(200);
        setIndexerVelocity(200);
        //intakeMode = intakeModes::loading; // Allows mode to be set to off while waiting
        pros::delay(200);
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

      case intakeModes::out:
      {
        setIntakeVelocity(-200);
        setIndexerVelocity(0);
        break;
      }

    }

    pros::delay(20);
  }

}
