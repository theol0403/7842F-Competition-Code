#pragma once
#include "main.h"


class IntakeController
{

public:

  enum intakeStates
  {
    off,
    loading, //Ball into indexer
    collecting, //Intake running
    shootIndexer,
    shootBoth,
    outIntake,
    outBoth,
    outSlow,
    compress
  };

  AbstractMotor* intake = nullptr;
  AbstractMotor* indexer = nullptr;
  pros::ADILineSensor* lineSensor = nullptr;
  okapi::EmaFilter sensorFilter;
  pros::Task intakeTask;

  intakeStates intakeState;

  IntakeController(AbstractMotor*, AbstractMotor*, pros::ADILineSensor*, double);

  void setState(intakeStates);

  void run();

  static void task(void*);

};
