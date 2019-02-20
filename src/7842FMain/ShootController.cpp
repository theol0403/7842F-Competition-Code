#include "ShootController.hpp"

ShootController::ShootController(lib7842::OdomTracker* itracker, IntakeController* iintake, FlywheelController* iflywheel, pros::ADIPotentiometer* ihoodSensor, double i0) :
tracker(itracker), intake(iintake), flywheel(iflywheel), hoodSensor(ihoodSensor), 0(i0),
shootTask(task, this)
{
}

void ShootController::clearQueue() {
  stateQueue.clear();
  stateQueue.push_back(shootStates::standby);
}

void ShootController::addJob(shootStates state) { stateQueue.push_back(state); }
ShootController::shootStates ShootController::getCurrentJob() { return stateQueue.back(); }

void ShootController::completeJob() {
  if(stateQueue.back() != shootStates::standby) {
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


double ShootController::getAngle() { return (hoodSensor->get_value() / 4095.0 * 265.0) - backAngle; }

QLength ShootController::getDistanceToFlag() {
  return 11_ft - tracker->state.y;
}

double ShootController::getTopFlagAngle() {
  switch((int)getDistanceToFlag().convert(inch))
  {
    case 0 ... 12 :
    {
      return 0;
      break;
    }
    case 13 ... 24 :
    {
      return 30;
      break;
    }
    case 25 ... 36 :
    {
      return 60;
      break;
    }
    case 37 ... 48 :
    {
      return 90;
      break;
    }
    default :
    {
      return 100;
      break;
    }
  }
  return 0;
}

double ShootController::getMiddleFlagAngle() {
  switch((int)getDistanceToFlag().convert(inch))
  {
    case 0 ... 12 :
    {
      return 0;
      break;
    }
    case 13 ... 24 :
    {
      return 60;
      break;
    }
    case 25 ... 36 :
    {
      return 90;
      break;
    }
    case 37 ... 48 :
    {
      return 120;
      break;
    }
    default :
    {
      return 100;
      break;
    }
  }
  return 0;
}

void ShootController::run()
{
  const double angleThresh = 30;

  while(true)
  {

    switch(getCurrentJob())
    {
      case shootStates::standby:
      {
        if(getAngle() > angleThresh) {
          addJob(shootStates::cycle);
        } else {
          flywheel->enable();
        }
        break;
      }

      case shootStates::cycle:
      {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(-50);
        if(getAngle() > 0) {
          flywheel->enable();
          completeJob();
        }
        break;
      }

      case shootStates::angleTop:
      {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(-50);
        if(getAngle() > getTopFlagAngle()) {
          flywheel->enable();
          completeJob();
        }
        break;
      }

      case shootStates::angleMiddle:
      {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(-50);
        if(getAngle() > getMiddleFlagAngle()) {
          flywheel->enable();
          completeJob();
        }
        break;
      }

      case shootStates::waitForFlywheel:
      {
        flywheel->enable();
        if(flywheel->pid->getError() < 100) {
          completeJob();
        }
        break;
      }

      case shootStates::shootIndexer:
      {
        flywheel->enable();
        intake->setState(IntakeController::shootIndexer);
        if(intake->getState() != IntakeController::shootIndexer) completeJob();
        break;
      }

      case shootStates::shootBoth:
      {
        flywheel->enable();
        intake->setState(IntakeController::shootBoth);
        if(intake->getState() != IntakeController::shootBoth) completeJob();
        break;
      }

    }

    pros::delay(10);
  }

}


void ShootController::task(void* input)
{
  ShootController* that = static_cast<ShootController*>(input);
  that->run();
}
