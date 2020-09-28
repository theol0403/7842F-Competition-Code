#pragma once
#include "main.h"

class IntakeController {

public:
  enum intakeStates {
    off,
    intakeBall, // runs intake and indexer until ball then indexer stops
    outIntake,
    outSlow,
    shoot
  };

  Motor* intake = nullptr;
  Motor* indexer = nullptr;
  pros::ADILineSensor* lineSensor = nullptr;
  okapi::EmaFilter sensorFilter;

  pros::Task task;

  intakeStates intakeState = off;
  bool hasBall = false;
  bool disabled = false;
  bool indexerSlave = true;

  IntakeController(Motor*, Motor*, pros::ADILineSensor*, double);

  void setState(intakeStates);
  intakeStates getState();
  void disable();
  void enable();

  void run();
  static void taskFnc(void*);
};
