#include "ShootController.hpp"

ShootController::ShootController(IntakeController* iintake, FlywheelController* iflywheel, pros::ADIPotentiometer* ihoodSensor, double ibackAngle) :
intake(iintake), flywheel(iflywheel), hoodSensor(ihoodSensor), backAngle(ibackAngle),
shootTask(task, this)
{
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
  currentJob = state;
  stateQueue.push_back(state);
}

void ShootController::addJobs(std::vector<shootStates> states) {
  for(shootStates &state : states) addJob(state);
}

void ShootController::addJobLoop(shootStates state) {
  addJob(loopJob);
  addJob(state);
}

void ShootController::addMacro(shootMacros macro) {
  currentMacro = macro;
  switch(macro) //these need to be reversed because the last one gets done first
  {
    case shootMacros::off :
    clearQueue();
    break;

    case shootMacros::shootTopFlag :
    addJobs({reportDone, shootIndexer, angleTop});
    break;

    case shootMacros::shootMiddleFlag :
    addJobs({reportDone, shootIndexer, angleMiddle});
    break;

    case shootMacros::shootTarget :
    addJobs({reportDone, shootIndexer, angleTarget});
    break;

    case shootMacros::shootBothFlags :
    addJobs({reportDone, shootIndexer, shootIndexer, angleMiddle, shootIndexer, angleTop});
    break;

    case shootMacros::shoot :
    addJobs({shootIndexer});
    break;

    case shootMacros::angle :
    addJobs({angling});
    break;
  }
}

void ShootController::addMacroLoop(shootMacros macro) {
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
  while(!macroCompleted) pros::delay(20);
}


double ShootController::getHoodAngle() {
  return (hoodSensor->get_value() / 4095.0 * 265.0) - backAngle;
}

// range is about 60 deg
double ShootController::getTopFlagAngle() {
  double x = distanceToFlag.convert(foot);
  // switch((int)x)
  // {
  //   case 0 ... 1 : return 0; break;
  //   case 2 ... 3 : return 5; break;
  //   case 4 ... 5 : return 15; break;
  //   case 6 ... 7 : return 25; break;
  //   case 8 ... 9 : return 35; break;
  //   default : return 0; break;
  // }
  double y = -0.0974*std::pow(x, 2) + 1.8549*x - 1.9294;
  if(x < 3.2) y = 0;
  if(y < 0 || y > 40) y = 0;
  return y;
}

double ShootController::getMiddleFlagAngle() {
  double x = distanceToFlag.convert(foot);
  // switch((int) distanceToFlag.convert(foot))
  // {
  //   case 0 ... 1 : return 0; break;
  //   case 2 ... 3 : return 15; break;
  //   case 4 ... 5 : return 25; break;
  //   case 6 ... 7 : return 35; break;
  //   case 8 ... 9 : return 45; break;
  //   default : return 20; break;
  // }
  double y = -0.2569*std::pow(x, 2) + 1.1859*x + 25.095;
  if(x <= 2) y = 20;
  if(x >= 10) y = 15;
  if(y < 0 || y > 40) y = 0;
  return y;
}


void ShootController::setTarget(double target) {
  targetAngle = target;
}

void ShootController::setDistanceToFlag(QLength distance) {
  distanceToFlag = distance;
}


void ShootController::run()
{
  const double angleThresh = 4;
  const double anglePower = -60;
  const double cycleVel = -60;

  const double extendPos = 40;

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
      flywheel->flywheel->move(anglePower);
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
      if(getHoodAngle() < extendPos) {
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
          flywheel->flywheel->move(anglePower);
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
          flywheel->flywheel->move(anglePower);
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
          flywheel->flywheel->move(anglePower);
        }
      }
      break;


      case waitForBall:
      flywheel->enable();
      intake->disable();
      intake->indexerSlave = false;
      if(intake->hasBall) {
        intake->enable();
        completeJob();
      } else {
        intake->intake->moveVelocity(200);
        intake->indexer->moveVelocity(80);
      }
      break;


      case waitForFlywheel:
      flywheel->enable();
      intake->indexer->move(0);
      intake->disable();
      intake->indexerSlave = true;
      if(flywheel->pid->getError() < 300) completeJob();
      break;


      case shootIndexer:
      flywheel->enable();
      intake->disable();
      pros::delay(10);
      intake->indexerSlave = false;
      intake->intake->moveVelocity(0);
      intake->indexer->moveVelocity(200);
      pros::delay(250);
      intake->enable();
      completeJob();
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
    pros::delay(10);
  }

}


void ShootController::task(void* input)
{
  ShootController* that = static_cast<ShootController*>(input);
  that->run();
}
