#include "ShootController.hpp"

ShootController::ShootController(lib7842::OdomTracker* itracker, IntakeController* iintake, FlywheelController* iflywheel, pros::ADIPotentiometer* ihoodSensor, double ibackAngle) :
tracker(itracker), intake(iintake), flywheel(iflywheel), hoodSensor(ihoodSensor), backAngle(ibackAngle),
shootTask(task, this)
{
}

void ShootController::clearQueue() {
  stateQueue.clear();
  stateQueue.push_back(standby);
}

void ShootController::addJob(shootStates state) { stateQueue.push_back(state); }
ShootController::shootStates ShootController::getCurrentJob() { return stateQueue.back(); }

void ShootController::completeJob() {
  if(stateQueue.back() != standby) {
    stateQueue.pop_back();
  }
}

void ShootController::addJobs(std::vector<shootStates> states) {
  for(shootStates &state : states) { stateQueue.push_back(state); }
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
  switch(macro)
  {
    case shootMacros::off :
    {
      clearQueue();
      break;
    }

    case shootMacros::shootTopFlag :
    {
      doJobs({angleTop, waitForFlywheel, shootIndexer});
      break;
    }

    case shootMacros::shootMiddleFlag :
    {
      doJobs({angleMiddle, waitForFlywheel, shootIndexer});
      break;
    }

    case shootMacros::shootBothFlags :
    {
      doJobs({angleTop, waitForFlywheel, shootIndexer, angleMiddle, waitForFlywheel, shootIndexer});
      break;
    }
  }
}


double ShootController::getAngle() { return (hoodSensor->get_value() / 4095.0 * 265.0) - backAngle; }

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

  while(true)
  {

    switch(getCurrentJob())
    {

      case off:
      flywheel->disable();
      break;

      case standby:

      if(getAngle() >= angleThresh) {
        addJob(cycle);
      } else {
        flywheel->enable();
      }
      break;

      case cycle:

      flywheel->disable();
      flywheel->flywheel->moveVelocity(-50);
      //If angle is within error of 0
      if(getAngle() <= angleThresh) {
        flywheel->enable();
        completeJob();
      }
      break;


      case angleTop:

      if(getAngle() > getTopFlagAngle() + angleThresh) {
        addJob(cycle);
      } else {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(-50);
        if(getAngle() >= getTopFlagAngle()) {
          flywheel->enable();
          completeJob();
        }
      }
      break;


      case angleMiddle:

      if(getAngle() > getMiddleFlagAngle() + angleThresh) {
        addJob(cycle);
      } else {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(-50);
        if(getAngle() >= getMiddleFlagAngle()) {
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
      intake->setState(IntakeController::shootIndexer);
      if(intake->getState() != IntakeController::shootIndexer) completeJob();
      break;


      case shootBoth:

      flywheel->enable();
      intake->setState(IntakeController::shootBoth);
      if(intake->getState() != IntakeController::shootBoth) completeJob();
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
