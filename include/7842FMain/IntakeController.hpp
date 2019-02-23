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
    outIntake
  };

  Motor* intake = nullptr;
  Motor* indexer = nullptr;
  pros::ADILineSensor* lineSensor = nullptr;
  okapi::EmaFilter sensorFilter;
  pros::Task intakeTask;

  intakeStates intakeState = off;
  bool disabled = false;
  bool indexerSlave = true;

  IntakeController(Motor*, Motor*, pros::ADILineSensor*, double);

  void setState(intakeStates);
  intakeStates getState();
  void disable();
  void enable();

  void run();

  static void task(void*);

};
