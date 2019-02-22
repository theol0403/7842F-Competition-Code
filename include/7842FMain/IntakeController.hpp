#pragma once
#include "main.h"
#include "FlywheelController.hpp"


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
  FlywheelController* flywheel = nullptr;
  pros::Task intakeTask;

  intakeStates intakeState = off;
  bool disabled = false;

  IntakeController(AbstractMotor*, AbstractMotor*, pros::ADILineSensor*, double, FlywheelController*);

  void setState(intakeStates);
  intakeStates getState();
  void disable();
  void enable();

  void run();

  static void task(void*);

};
