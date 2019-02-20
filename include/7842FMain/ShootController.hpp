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
    take
    angleTop, //drop hood to top angle
    angleMiddle, //drop hood to middle angle
    waitForFlywheel, //wait until flywheel is ready
    shoot, //shoot indexer
    cycle, //head to back position
  };

  lib7842::OdomTracker* tracker = nullptr;
  IntakeController* intake = nullptr;
  FlywheelController* flywheel = nullptr;
  ADIPotentiometer* hoodSensor = nullptr;
  double backAngle = 0;
  pros::Task shootTask;

  std::vector<shootStates> stateQueue = {standby};

  ShootController(OdomTracker*, IntakeController*, FlywheelController*, pros::ADIPotentiometer*, double);

  void clearQueue();
  void addJob(shootStates);
  shootStates getCurrentJob();
  void completeJob();

  double getAngle();
  QLength getDistanceToFlag();
  double getTopFlagAngle();
  double getMiddleFlagAngle();


  void run();
  static void task(void*);

};
