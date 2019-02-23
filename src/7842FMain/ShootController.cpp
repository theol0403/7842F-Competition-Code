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
  currentJob = state;
  stateQueue.push_back(state);
}

void ShootController::addJobs(std::vector<shootStates> states) {
  for(shootStates &state : states) { addJob(state); }
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


double ShootController::getHoodAngle() {
  return (hoodSensor->get_value() / 4095.0 * 265.0) - backAngle;
}

QLength ShootController::getDistanceToFlag() {
  return 11_ft - tracker->state.y;
}

double ShootController::getTopFlagAngle() {
  // switch((int) getDistanceToFlag().convert(foot))
  // {
  //   case 0 : return 0; break;
  //   case 1 : return 30; break;
  //   case 2 : return 60; break;
  //   case 3 : return 90; break;
  //   case 4 : return 120; break;
  //   default : return 150; break;
  // }
  return 0;
}

double ShootController::getMiddleFlagAngle() {
  // switch((int) getDistanceToFlag().convert(foot))
  // {
  //   case 0 : return 30; break;
  //   case 1 : return 60; break;
  //   case 2 : return 90; break;
  //   case 3 : return 120; break;
  //   case 4 : return 150; break;
  //   default : return 180; break;
  // }
  return 20;
}



void ShootController::run()
{
  const double angleThresh = 6;
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
      intake->enable();
      flywheel->disable();
      //flywheel->flywheel->moveVelocity(-30);
      flywheel->flywheel->move(angleSpeed);
      break;

      case standby:
      if(getHoodAngle() >= angleThresh) {
        addJob(cycle);
      } else {
        flywheel->enable();
        intake->enable();
      }
      break;

      case cycle:
      //std::cout << "Cycling!" << std::endl;
      intake->enable();
      flywheel->disable();
      flywheel->flywheel->moveVelocity(-30);
      //If angle is within error of 0
      if(getHoodAngle() <= angleThresh) {
        flywheel->enable();
        completeJob();
      }
      break;


      case angleTop:
      //std::cout << "Angling!" << std::endl;
      if(getHoodAngle() > getTopFlagAngle() + angleThresh) {
        addJob(cycle);
      } else {
        if(getHoodAngle() >= getTopFlagAngle() - angleThresh) {
          std::cout << "Exit!" << std::endl;
          flywheel->enable();
          completeJob();
        } else {
          intake->enable();
          flywheel->disable();
          flywheel->flywheel->move(angleSpeed);
        }
      }
      break;


      case angleMiddle:
      //std::cout << "Angling!" << std::endl;
      if(getHoodAngle() > getMiddleFlagAngle() + angleThresh) {
        addJob(cycle);
      } else {
        if(getHoodAngle() >= getMiddleFlagAngle() - angleThresh) {
          //std::cout << "Exit!" << std::endl;
          flywheel->enable();
          completeJob();
        } else {
          intake->enable();
          flywheel->disable();
          flywheel->flywheel->move(angleSpeed);
        }
      }
      break;


      case waitForFlywheel:
      flywheel->enable();
      intake->enable();
      if(flywheel->pid->getError() < 100) completeJob();
      break;


      case shootIndexer:
      flywheel->enable();
      intake->disable();
      intake->indexerSlave = false;
      intake->intake->moveVelocity(0);
      intake->indexer->moveVelocity(200);
      pros::delay(300);
      //intake->enable();
      completeJob();
      break;


      case shootBoth:
      flywheel->enable();
      intake->disable();
      intake->indexerSlave = false;
      intake->intake->moveVelocity(200);
      intake->indexer->moveVelocity(200);
      pros::delay(300);
      //intake->enable();
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
