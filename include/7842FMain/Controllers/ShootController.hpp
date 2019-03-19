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
    standby, //back position, control to flywheel
    angling, //indefinite angling
    cycle, //head to back position
    extend, //move to extended position
    waitForSlip, //wait until hood slips
    waitForRetract, //wait until hood back to 0 pos
    angleTop, //drop hood to top angle
    angleMiddle, //drop hood to middle angle
    angleTarget, //drop hood to target angle
    waitForBall, //wait for ball to be in indexer
    waitForFlywheel, //wait until flywheel is ready
    shootIndexer, //shoot indexer
    reportDone, //lets autonomous know
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
    angle
  };

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
  QLength distanceToFlag = 0_in;
  bool macroCompleted = false;

  IterativePosPIDController* hoodPid = nullptr;

  ShootController(IntakeController*, FlywheelController*, pros::ADIPotentiometer*, double, IterativePosPIDController*);

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
  void doMacroBlocking(shootMacros);

  double getHoodAngle();
  double getTopFlagAngle();
  double getMiddleFlagAngle();

  void setTarget(double);
  void setDistanceToFlag(QLength);

  double computeHoodPower(double);

  void run();
  static void task(void*);

};