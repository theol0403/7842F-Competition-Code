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
    off,
    standby, //back position, control to flywheel
    cycle, //head to back position
    angleTop, //drop hood to top angle
    angleMiddle, //drop hood to middle angle
    waitForFlywheel, //wait until flywheel is ready
    shootIndexer, //shoot indexer
    shootBoth, //shoot both
    loopMacro
  };

  enum class shootMacros
  {
    off, //reverts to standby
    shootTopFlag,
    shootMiddleFlag,
    shootBothFlags,
    shoot
  };

  lib7842::OdomTracker* tracker = nullptr;
  IntakeController* intake = nullptr;
  FlywheelController* flywheel = nullptr;
  pros::ADIPotentiometer* hoodSensor = nullptr;
  double backAngle = 0;
  pros::Task shootTask;

  std::vector<shootStates> stateQueue = {shootStates::standby};
  shootMacros currentMacro = shootMacros::off;

  ShootController(lib7842::OdomTracker*, IntakeController*, FlywheelController*, pros::ADIPotentiometer*, double);

  void clearQueue();
  shootStates getCurrentJob();
  void completeJob();

  void addJob(shootStates);
  void addJobs(std::vector<shootStates>);
  void doJob(shootStates);
  void doJobs(std::vector<shootStates>);

  void addMacro(shootMacros);
  void doMacro(shootMacros);
  void addMacroLoop(shootMacros);
  void doMacroLoop(shootMacros);

  double getAngle();
  QLength getDistanceToFlag();
  double getTopFlagAngle();
  double getMiddleFlagAngle();

  void run();
  static void task(void*);

};
