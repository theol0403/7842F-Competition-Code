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
    off, //control to flywheel and intake
    angling, //indefinite angling
    standby, //back position, control to flywheel
    cycle, //head to back position
    angleTop, //drop hood to top angle
    angleMiddle, //drop hood to middle angle
    angleTarget, //drop hood to target angle
    waitForFlywheel, //wait until flywheel is ready
    shootIndexer, //shoot indexer
    shootBoth, //shoot both
    loopJob, //reloads current job
    loopMacro //reloads current macro
  };

  enum class shootMacros
  {
    off,
    shootTopFlag,
    shootMiddleFlag,
    shootBothFlags,
    shootTarget,
    shoot,
    angleManual
  };

  lib7842::OdomTracker* tracker = nullptr;
  IntakeController* intake = nullptr;
  FlywheelController* flywheel = nullptr;
  pros::ADIPotentiometer* hoodSensor = nullptr;
  double backAngle = 0;
  pros::Task shootTask;

  const shootStates defaultState = standby;

  std::vector<shootStates> stateQueue = {defaultState};
  shootStates currentJob = defaultState;
  shootMacros currentMacro = shootMacros::off;
  double targetAngle = 0;

  ShootController(lib7842::OdomTracker*, IntakeController*, FlywheelController*, pros::ADIPotentiometer*, double);

  void clearQueue();
  void completeJob();
  shootStates getCurrentJob();

  void addJob(shootStates);
  void addJobs(std::vector<shootStates>);
  void addJobLoop(shootStates);
  void addMacro(shootMacros);
  void addMacroLoop(shootMacros);

  void doJob(shootStates);
  void doJobs(std::vector<shootStates>);
  void doJobLoop(shootStates);
  void doMacro(shootMacros);
  void doMacroLoop(shootMacros);

  double getHoodAngle();
  QLength getDistanceToFlag();
  double getTopFlagAngle();
  double getMiddleFlagAngle();

  void setTarget(double);

  void run();
  static void task(void*);

};
