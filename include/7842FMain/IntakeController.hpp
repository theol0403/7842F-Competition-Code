#pragma once
#include "main.h"

class IntakeController
{

public:

  enum intakeStates
  {
    off,
    loadBall,
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
