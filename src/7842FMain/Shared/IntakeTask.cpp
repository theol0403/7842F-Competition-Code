#include "IntakeTask.hpp"




bool runIntake = false;
bool currentIntake = runIntake;

intakeModes intakeMode = intakeModes::loading;

void intakeControlTask(void*)
{


  while(true)
  {

    //if runIntake is new mode is loading
    //if runIntake is off mode is off
    if(runIntake && !currentIntake)
    {
      intakeMode = intakeModes::loading;
      currentIntake = true;
    }
    else if(!runIntake && currentIntake)
    {
      intakeMode = intakeModes::off;
      currentIntake = false;
    }


    switch(intakeMode)
    {

      case intakeModes::loading:
      {
        //Intake and collect until ball is in sensor
        //once ball is in sensor index brake
        //collecting
        setIntakePower(100);
        setIndexerVelocity(100);
        if(getIndexerSensor() < -300)
        {
          setIndexerVelocity(0);
          intakeMode = intakeModes::collecting;
        }
        break;
      }

      case intakeModes::collecting:
      {
        //Run intake
        setIntakePower(100);
        break;
      }

      case intakeModes::shoot:
      {
        setIndexerVelocity(200);
        pros::delay(200);
        intakeMode = intakeModes::loading;
        break;
      }

      case intakeModes::off:
      {
        setIndexerVelocity(0);
        setIntakePower(0);
        break;
      }

    }

    pros::delay(20);
  }

}
