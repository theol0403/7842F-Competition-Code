#pragma once
#include "main.h"
#include "FlywheelController.hpp"
#include "IntakeController.hpp"
#include "lib7842/odometry/tracker.hpp"


class ShootController
{

public:

  enum shootStates
  {
    standby, //back position, control to flywheel
    angleTop, //drop hood to top angle
    angleBottom, //drop hood to bottom angle
    waitForFlywheel, //wait until flywheel is ready
    shoot, //shoot indexer
    cycle, //head to back position
  };

  OdomTracker* tracker = nullptr;
  IntakeController* intake = nullptr;
  FlywheelController* flywheel = nullptr;
  pros::Task shootTask;

  std::vector<shootStates> stateQueue = {standby};

  ShootController(OdomTracker*, IntakeController*, FlywheelController*);

  void clearQueue();
  void addQueue(shootStates);
  void setState(shootStates);

  void run();
  static void task(void*);

};
