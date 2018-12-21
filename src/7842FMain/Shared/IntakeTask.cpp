#include "IntakeTask.hpp"


enum class intakeModes
{
  loading,
  collecting,
  off,
  shoot
};

bool runIntake;

void intakeControlTask(void*)
{
  intakeModes intakeMode = intakeModes::loading;

  while(true)
  {
    //if runIntake is new mode is loading
    //if runIntake is off mode is off

    switch(intakeMode)
    {

      case intakeModes::loading:
      {
        //Intake and collect until ball is in sensor
        //once ball is in sensor index brake
        //collecting
        break;
      }

      case intakeModes::collecting:
      {
        //Run intake
        break;
      }

    }

    pros::delay(20);
  }

}
