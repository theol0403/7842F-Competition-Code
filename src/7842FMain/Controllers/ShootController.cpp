#include "ShootController.hpp"

ShootController::ShootController(IntakeController*& iintake, FlywheelController*& iflywheel, pros::ADIPotentiometer* ihoodSensor, double ibackAngle, IterativePosPIDController* ihoodPid) :
intake(iintake), flywheel(iflywheel), hoodSensor(ihoodSensor), backAngle(ibackAngle), hoodPid(ihoodPid),
task(taskFnc, this)
{
  topAngles = {
    {0, 0},
    {0.5, 0},
    {1, 0},
    {1.5, 0},
    {2, 0},
    {2.5, 0},
    {3, 0},
    {3.5, 0},
    {4, 2.2},
    {4.5, 4.9},
    {5, 6},
    {5.5, 5.3},
    {6, 6},
    {6.5, 6.5},
    {7, 7.1},
    {7.5, 5.2},
    {8, 6.1},
    {8.5, 3.5},
    {9, 2.9},
    {9.5, 3},
    {10, 2},
    {10.5, 1.5},
    {11, 1}
  };

  middleAngles = {
    {0, 0},
    {0.5, 0},
    {1, 0},
    {1.5, 0},
    {2, 0},
    {2.5, 3},
    {3, 9.8},
    {3.5, 12.6},
    {4, 13.4},
    {4.5, 15},
    {5, 15.1},
    {5.5, 13.3},
    {6, 13},
    {6.5, 14.2},
    {7, 15},
    {7.5, 14.4},
    {8, 11.9},
    {8.5, 10.8},
    {9, 10.9},
    {9.5, 8.6},
    {10, 8.9},
    {10.5, 7},
    {11, 4.6}
  };

}

void ShootController::clearQueue() {
  stateQueue.clear();
  stateQueue.push_back(defaultState);
}

void ShootController::completeJob() {
  if(stateQueue.back() != defaultState) {
    stateQueue.pop_back();
  }
}

ShootController::shootStates ShootController::getCurrentJob() {
  return stateQueue.back();
}

void ShootController::addJob(shootStates state) {
  stateQueue.push_back(state);
}

void ShootController::addJobs(std::vector<shootStates> states) {
  for(shootStates &state : states) addJob(state);
}

void ShootController::addJobLoop(shootStates state) {
  currentJob = state;
  addJob(loopJob);
  addJob(state);
}

void ShootController::addMacro(shootMacros macro) {
  switch(macro) //these need to be reversed because the last one gets done first
  {
    case shootMacros::off :
    clearQueue();
    break;

    case shootMacros::shootTop :
    addJobs({reportDone, enableShoot, angleTop});
    break;

    case shootMacros::shootMiddle :
    addJobs({reportDone, enableShoot, angleMiddle});
    break;

    case shootMacros::shootTarget :
    addJobs({reportDone, enableShoot, angleTarget});
    break;

    case shootMacros::shootOut :
    addJobs({reportDone, enableShoot, angleOut});
    break;

    case shootMacros::shootBoth :
    addJobs({reportDone, enableShoot, angleMiddle, enableShoot, angleTop});
    break;

    case shootMacros::shoot :
    addJobs({enableShoot});
    break;

    case shootMacros::angle :
    addJobs({angling});
    break;
  }
}

void ShootController::addMacroLoop(shootMacros macro) {
  currentMacro = macro;
  addJob(loopMacro);
  addMacro(macro);
}


void ShootController::doJob(shootStates state) {
  clearQueue();
  addJob(state);
}

void ShootController::doJobs(std::vector<shootStates> states) {
  clearQueue();
  addJobs(states);
}

void ShootController::doJobLoop(shootStates state) {
  clearQueue();
  addJobLoop(state);
}

void ShootController::doMacro(shootMacros macro) {
  clearQueue();
  addMacro(macro);
}

void ShootController::doMacroLoop(shootMacros macro) {
  clearQueue();
  addMacroLoop(macro);
}

void ShootController::doMacroBlocking(shootMacros macro) {
  macroCompleted = false;
  doMacro(macro);
  while(!macroCompleted) pros::delay(10);
}


double ShootController::getHoodAngle() {
  return (hoodSensor->get_value() / 4095.0 * 265.0) - backAngle;
}


double ShootController::getTopFlagAngle() {
  double x = distanceToFlag.convert(foot);
  x = std::round(x * 2) / 2; //round to nearest 0.5
  if(x < 0 || x > 11) return 0; //bounds checking
  double y = topAngles.at(x); //hopefully there is angle with distance key
  //std::cout << "Distance: " << x << " Angle: " << y << std::endl;
  return y;
}

double ShootController::getMiddleFlagAngle() {
  double x = distanceToFlag.convert(foot);
  x = std::round(x * 2) / 2; //round to nearest 0.5
  if(x < 0 || x > 11) return 0; //bounds checking
  double y = middleAngles.at(x); //hopefully there is angle with distance key
  //std::cout << "Distance: " << x << " Angle: " << y << std::endl;
  return y;
}


void ShootController::setTarget(double target) {
  targetAngle = target;
}

void ShootController::setDistanceToFlag(QLength distance) {
  distanceToFlag = distance;
}

double ShootController::computeHoodPower(double target) {
  hoodPid->setTarget(target);
  double output = hoodPid->step(getHoodAngle()) * 127;
  if(output < 0) {
    output = 0;
    std::cerr << "computeHoodPower: angler pid < 0" << std::endl;
  }
  //if(output > 100) output = 100;
  if(output < 40) output = 40;
  return output;
}

void ShootController::run()
{
  const double angleThresh = 2;
  const double cycleVel = -50;

  const double extendPos = 45;

  Timer shootTimer;

  while(true)
  {

    switch(getCurrentJob())
    {

      case off:
      flywheel->enable();
      intake->enable();
      break;

      case standby:
      if(getHoodAngle() > angleThresh) {
        addJob(cycle);
      } else {
        doJob(off);
      }
      break;

      case angling:
      intake->enable();
      flywheel->disable();
      flywheel->flywheel->move(-60);
      break;

      case cycle:
      completeJob();
      addJobs({waitForRetract, waitForSlip, extend});
      break;

      case extend:
      intake->enable();
      flywheel->disable();
      flywheel->flywheel->moveVelocity(cycleVel);
      if(getHoodAngle() > extendPos) {
        flywheel->enable();
        completeJob();
      }
      break;

      case waitForSlip:
      intake->enable();
      flywheel->disable();
      flywheel->flywheel->moveVelocity(cycleVel);
      if(getHoodAngle() <= extendPos) {
        flywheel->enable();
        completeJob();
      }
      break;

      case waitForRetract:
      intake->enable();
      flywheel->enable();
      if(getHoodAngle() < angleThresh) {
        completeJob();
      }
      break;


      case angleTop:
      if(getHoodAngle() > getTopFlagAngle() + angleThresh) {
        addJob(cycle);
      } else {
        if(getHoodAngle() >= getTopFlagAngle() - angleThresh) {
          flywheel->enable();
          completeJob();
        } else {
          intake->enable();
          flywheel->disable();
          flywheel->flywheel->move(-computeHoodPower(getTopFlagAngle()));
        }
      }
      break;


      case angleMiddle:
      if(getHoodAngle() > getMiddleFlagAngle() + angleThresh) {
        addJob(cycle);
      } else {
        if(getHoodAngle() >= getMiddleFlagAngle() - angleThresh) {
          flywheel->enable();
          completeJob();
        } else {
          intake->enable();
          flywheel->disable();
          flywheel->flywheel->move(-computeHoodPower(getMiddleFlagAngle()));
        }
      }
      break;


      case angleTarget:
      if(getHoodAngle() > targetAngle + angleThresh) {
        addJob(cycle);
      } else {
        if(getHoodAngle() >= targetAngle - angleThresh) {
          flywheel->enable();
          completeJob();
        } else {
          intake->enable();
          flywheel->disable();
          flywheel->flywheel->move(-computeHoodPower(targetAngle));
        }
      }
      break;


      case angleOut:
      if(getHoodAngle() > 32 + angleThresh) {
        addJob(cycle);
      } else {
        if(getHoodAngle() >= 32 - angleThresh) {
          flywheel->enable();
          completeJob();
        } else {
          intake->enable();
          flywheel->disable();
          flywheel->flywheel->move(-computeHoodPower(32));
        }
      }
      break;


      case waitForBall: {
        flywheel->enable();
        intake->disable();
        int count = 10;
        while(count > 0) {
          intake->intake->moveVelocity(200);
          intake->indexer->moveVelocity(200);
          intake->indexerSlave = false;
          count--;
          pros::delay(10);
        }
        intake->enable();
        completeJob();
        break;
      }


      case waitForFlywheel:
      flywheel->enable();
      intake->disable();
      intake->indexerSlave = true;
      if(flywheel->pid->getError() < 300) completeJob();
      break;


      case enableShoot:
      shootTimer.placeMark();
      completeJob();
      addJob(waitForShoot);
      break;

      case waitForShoot:
      if(shootTimer.getDtFromMark() >= 300_ms) {
        intake->enable();
        completeJob();
      } else {
        flywheel->enable();
        intake->disable();
        intake->intake->moveVelocity(50);
        intake->indexer->moveVelocity(200);
        intake->indexerSlave = false;
      }
      break;

      case reportDone:
      macroCompleted = true;
      completeJob();
      break;

      case loopJob:
      addJob(currentJob);
      break;

      case loopMacro:
      addMacro(currentMacro);
      break;

    }
    //std::cout << "Hood: " << getHoodAngle() << std::endl;
    //std::cout << (intake->indexerSlave && !flywheel->disabled) << std::endl;
    pros::delay(3);
  }

}


void ShootController::taskFnc(void* input)
{
  pros::delay(500);
  ShootController* that = static_cast<ShootController*>(input);
  that->run();
}
