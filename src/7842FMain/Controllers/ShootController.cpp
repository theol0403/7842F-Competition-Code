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
    {4, 3.2},
    {4.5, 8},
    {5, 7},
    {5.5, 5.3},
    {6, 6},
    {6.5, 6.5},
    {7, 6.5},
    {7.5, 5.2},
    {8, 6.1},
    {8.5, 14},
    {9, 14},
    {9.5, 10},
    {10, 8},
    {10.5, 8},
    {11, 13}
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
    {4, 21},
    {4.5, 22.4},
    {5, 15.1},
    {5.5, 13.3},
    {6, 13},
    {6.5, 14.2},
    {7, 19},
    {7.5, 14.4},
    {8, 15.5},
    {8.5, 26},
    {9, 24},
    {9.5, 21},
    {10, 18},
    {10.5, 18},
    {11, 23}
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
  for(auto&& state : states) addJob(state);
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

    case shootMacros::shootBoth :
    addJobs({reportDone, enableShoot, enableShoot, waitForDoubleShot, angleMiddle, enableShoot, angleTop});
    break;

    case shootMacros::shootTopPlatform :
    addJobs({reportDone, enableShoot, angleTopPlatform});
    break;

    case shootMacros::shootMiddlePlatform :
    addJobs({reportDone, enableShoot, angleMiddlePlatform});
    break;

    case shootMacros::shootBothPlatform :
    addJobs({reportDone, enableShoot, enableShoot, angleMiddlePlatform, enableShoot, angleTopPlatform});
    break;

    case shootMacros::shootOut :
    addJobs({reportDone, enableShoot, angleOut});
    break;

    case shootMacros::shootTarget :
    addJobs({reportDone, enableShoot, angleTarget});
    break;

    case shootMacros::shoot :
    addJobs({reportDone, enableShoot});
    break;

    case shootMacros::angle :
    addJobs({reportDone, angling});
    break;

    case shootMacros::cycle :
    addJobs({reportDone, cycle});
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
  macroCompleted = false;
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
  if(output < 40) output = 40;
  return output;
}

void ShootController::angleTo(double angle) {
  if(getHoodAngle() >= angle + 1) { //how much forward before cycle
    addJob(cycle);
  } else {
    if(getHoodAngle() >= angle - 4) { //how much back before stop
      flywheel->enable();
      completeJob();
    } else {
      intake->enable();
      flywheel->disable();
      flywheel->flywheel->move(-computeHoodPower(angle));
    }
  }
}


void ShootController::run()
{
  const double angleThresh = 2;
  const double cycleVel = -50;

  const double extendPos = 45; //a bit less than the angle right before it slips

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
      if(getHoodAngle() > extendPos) {
        flywheel->enable();
        completeJob();
      } else {
        intake->enable();
        flywheel->disable();
        flywheel->flywheel->moveVelocity(cycleVel);
      }
      break;

      case waitForSlip:
      if(getHoodAngle() <= extendPos) {
        flywheel->enable();
        completeJob();
      } else {
        intake->enable();
        flywheel->disable();
        flywheel->flywheel->moveVelocity(cycleVel);
      }
      break;

      case waitForRetract:
      intake->enable();
      flywheel->enable();
      if(getHoodAngle() < angleThresh) {
        completeJob();
      } else {
        intake->enable();
        flywheel->enable();
      }
      break;


      case angleTop:
      angleTo(getTopFlagAngle());
      break;

      case angleMiddle:
      angleTo(getMiddleFlagAngle());
      break;

      case angleTopPlatform:
      angleTo(20);
      break;

      case angleMiddlePlatform:
      angleTo(32);
      break;

      case angleOut:
      angleTo(40);
      break;

      case angleTarget:
      angleTo(targetAngle);
      break;


      case waitForDoubleShot:
      flywheel->enable();
      intake->enable();
      if(distanceToFlag > 6_ft) {
        pros::delay(500);
      }
      completeJob();
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
        intake->intake->moveVelocity(200);
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


void ShootController::taskFnc(void* input) {
  pros::delay(100);
  ShootController* that = static_cast<ShootController*>(input);
  that->run();
}
