#pragma once
#include "FlywheelController.hpp"
#include "IntakeController.hpp"
#include "lib7842/odometry/tracker.hpp"
#include "main.h"

class ShootController {

public:
  enum shootStates {
    off, // control to flywheel and intake
    standby, // back position, control to flywheel
    angling, // indefinite angling
    cycle, // head to back position
    extend, // move to extended position
    waitForSlip, // wait until hood slips
    waitForRetract, // wait until hood back to 0 pos
    angleTop, // drop hood to top angle
    angleMiddle, // drop hood to middle angle
    angleTopPlatform, // angle top from platform
    angleMiddlePlatform, // angle middle from platform
    angleOut, // drop hood to out (ground flag) angle
    angleTarget, // drop hood to target angle
    waitForDoubleShot, // if distance is large enough, wait before second shot
    waitForBall, // wait for ball to be in indexer
    waitForFlywheel, // wait until flywheel is ready
    enableShoot, // shoot indexer
    waitForShoot, // delays until shot detected
    reportDone, // lets autonomous know
    loopJob, // reloads current job
    loopMacro // reloads current macro
  };

  enum class shootMacros {
    off,
    shootTop,
    shootMiddle,
    shootBoth,
    shootTopPlatform,
    shootMiddlePlatform,
    shootBothPlatform,
    shootOut,
    shootTarget,
    shoot,
    angle,
    cycle
  };

  IntakeController*& intake;
  FlywheelController*& flywheel;
  pros::ADIPotentiometer* hoodSensor = nullptr;
  double backAngle = 0;
  IterativePosPIDController* hoodPid = nullptr;

  pros::Task task;

  std::map<float, double> topAngles = {};
  std::map<float, double> middleAngles = {};

  const shootStates defaultState = off;

  std::vector<shootStates> stateQueue = {defaultState};
  shootStates currentJob = defaultState;
  shootMacros currentMacro = shootMacros::off;
  double targetAngle = 0;
  QLength distanceToFlag = 0_in;
  bool macroCompleted = true;
  ;

  ShootController(IntakeController*&, FlywheelController*&, pros::ADIPotentiometer*, double,
                  IterativePosPIDController*);

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
  void angleTo(double);

  void run();
  static void taskFnc(void*);
};
