#include "ShootController.hpp"

ShootController::ShootController(lib7842::OdomTracker* itracker, IntakeController* iintake, FlywheelController* iflywheel, pros::ADIPotentiometer* ihoodSensor, double ibackAngle) :
tracker(itracker), intake(iintake), flywheel(iflywheel), hoodSensor(ihoodSensor), backAngle(ibackAngle),
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
  stateQueue.push_back(state);
}

void ShootController::addJobs(std::vector<shootStates> states) {
  for(shootStates &state : states) { addJob(state); }
}

void ShootController::addMacro(shootMacros macro) {
  switch(macro) //these need to be reversed because the last one gets done first
  {
    case shootMacros::off :
    clearQueue();
    break;

    case shootMacros::shootTopFlag :
    addJobs({shootIndexer, waitForFlywheel, angleTop});
    break;

    case shootMacros::shootMiddleFlag :
    addJobs({shootIndexer, waitForFlywheel, angleMiddle});
    break;

    case shootMacros::shootBothFlags :
    addJobs({shootIndexer, waitForFlywheel, angleMiddle, shootIndexer, waitForFlywheel, angleTop});
    break;

    case shootMacros::shoot :
    addJobs({shootIndexer, waitForFlywheel});
    break;

    case shootMacros::angleManual :
    addJobs({angling});
    break;
  }
}

void ShootController::addJobLoop(shootStates state) {
  currentJobLoop = state;
  addJob(loopJob);
  addJob(state);
}

void ShootController::addMacroLoop(shootMacros macro) {
  currentMacroLoop = macro;
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

void ShootController::doMacro(shootMacros macro) {
  clearQueue();
  addMacro(macro);
}

void ShootController::doJobLoop(shootStates state) {
  clearQueue();
  addJobLoop(state);
}

void ShootController::doMacroLoop(shootMacros macro) {
  clearQueue();
  addMacroLoop(macro);
}


double ShootController::getHoodAngle() {
  return (hoodSensor->get_value() / 4095.0 * 265.0) - backAngle;
}

QLength ShootController::getDistanceToFlag() {
  return 11_ft - tracker->state.y;
}

double ShootController::getTopFlagAngle() {
  switch((int) getDistanceToFlag().convert(foot))
  {
    case 0 : return 0; break;
    case 1 : return 30; break;
    case 2 : return 60; break;
    case 3 : return 90; break;
    case 4 : return 120; break;
    default : return 150; break;
  }
  return 0;
}

double ShootController::getMiddleFlagAngle() {
  switch((int) getDistanceToFlag().convert(foot))
  {
    case 0 : return 30; break;
    case 1 : return 60; break;
    case 2 : return 90; break;
    case 3 : return 120; break;
    case 4 : return 150; break;
    default : return 180; break;
  }
  return 0;
}



void ShootController::run()
{
  const double angleThresh = 15;
  const double angleSpeed = -50;

  while(true)
  {

    switch(getCurrentJob())
    {

      case off:
      flywheel->enable();
      intake->enable();
      break;

      case angling:
      flywheel->disable();
      flywheel->flywheel->moveVelocity(angleSpeed);
      break;

      case standby:
      if(getHoodAngle() >= angleThresh) {
        addJob(cycle);
      } else {
        flywheel->enable();
      }
      break;

      case cycle:
      flywheel->disable();
      flywheel->flywheel->moveVelocity(angleSpeed);
      //If angle is within error of 0
      if(getHoodAngle() <= angleThresh) {
        flywheel->enable();
        completeJob();
      }
      break;


      case angleTop:
      if(getHoodAngle() > getTopFlagAngle() + angleThresh) {
        addJob(cycle);
      } else {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(angleSpeed);
        if(getHoodAngle() >= getTopFlagAngle()) {
          flywheel->enable();
          completeJob();
        }
      }
      break;


      case angleMiddle:
      if(getHoodAngle() > getMiddleFlagAngle() + angleThresh) {
        addJob(cycle);
      } else {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(angleSpeed);
        if(getHoodAngle() >= getMiddleFlagAngle()) {
          flywheel->enable();
          completeJob();
        }
      }
      break;


      case waitForFlywheel:
      flywheel->enable();
      if(flywheel->pid->getError() < 100) completeJob();
      break;


      case shootIndexer:
      flywheel->enable();
      intake->disable();
      intake->intake->moveVelocity(0);
      intake->indexer->moveVelocity(200);
      pros::delay(300);
      intake->enable();
      completeJob();
      break;


      case shootBoth:
      flywheel->enable();
      intake->disable();
      intake->intake->moveVelocity(200);
      intake->indexer->moveVelocity(200);
      pros::delay(600);
      intake->enable();
      completeJob();
      break;

      case loopJob:
      addJob(currentJobLoop);
      break;

      case loopMacro:
      addMacro(currentMacroLoop);
      break;

    }

    pros::delay(10);
  }

}


void ShootController::task(void* input)
{
  ShootController* that = static_cast<ShootController*>(input);
  that->run();
}
