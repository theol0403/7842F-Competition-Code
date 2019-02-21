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
  bool disabled = false;

  IntakeController(AbstractMotor*, AbstractMotor*, pros::ADILineSensor*, double);

  void setState(intakeStates);
  intakeStates getState();
  void disable();
  void enable();

  void run();

  static void task(void*);

};
