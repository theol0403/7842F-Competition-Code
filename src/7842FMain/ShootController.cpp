#include "ShootController.hpp"

ShootController::ShootController(lib7842::OdomTracker* itracker, IntakeController* iintake, FlywheelController* iflywheel, pros::ADIPotentiometer* ihoodSensor, double ibackAngle) :
tracker(itracker), intake(iintake), flywheel(iflywheel), hoodSensor(ihoodSensor), backAngle(ibackAngle),
shootTask(task, this)
{
}

void ShootController::clearQueue() {
  stateQueue.clear();
  stateQueue.push_back({standby});
}
void ShootController::addJob(shootStates state) {
  stateQueue.push_back(state);
}
ShootController::shootStates ShootController::getCurrentJob() {
  return stateQueue.back();
}
void ShootController::completeJob() {
  if(stateQueue.back() != standby) {
    stateQueue.pop_back();
  }
}

double ShootController::getAngle() {
  return hoodSensor->get_value() / 4095.0 * 265.0;
}

QLength ShootController::getDistanceToFlag() {
  return 11_ft - tracker->state.y;
}

double ShootController::getTopFlagAngle() {
  switch((int)getDistanceToFlag().convert(inch))
  {
    case 0 ... 12 :
    {
      return backAngle;
      break;
    }
    case 13 ... 24 :
    {
      return backAngle + 30;
      break;
    }
    case 25 ... 36 :
    {
      return backAngle + 60;
      break;
    }
    case 37 ... 48 :
    {
      return backAngle + 90;
      break;
    }
  }
  return backAngle;
}

double ShootController::getMiddleFlagAngle() {
  switch((int)getDistanceToFlag().convert(inch))
  {
    case 0 ... 12 :
    {
      return backAngle;
      break;
    }
    case 13 ... 24 :
    {
      return backAngle + 60;
      break;
    }
    case 25 ... 36 :
    {
      return backAngle + 90;
      break;
    }
    case 37 ... 48 :
    {
      return backAngle + 120;
      break;
    }
  }
  return backAngle;
}

void ShootController::run()
{
  const double angleThresh = 30;

  while(true)
  {

    switch(getCurrentJob())
    {
      case standby:
      {
        if(getAngle() > backAngle + angleThresh) {
          addJob(cycle);
        } else {
          flywheel->enable();
        }
        break;
      }

      case cycle:
      {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(-50);
        if(getAngle() > backAngle) {
          flywheel->enable();
          completeJob();
        }
        break;
      }

      case angleTop:
      {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(-50);
        if(getAngle() > getTopFlagAngle()) {
          flywheel->enable();
          completeJob();
        }
        break;
      }

      case angleMiddle:
      {
        flywheel->disable();
        flywheel->flywheel->moveVelocity(-50);
        if(getAngle() > getMiddleFlagAngle()) {
          flywheel->enable();
          completeJob();
        }
        break;
      }

      case waitForFlywheel:
      {
        flywheel->enable();
        if(flywheel->pid->getError() < 100) {
          completeJob();
        }
        break;
      }

      case shootIndexer:
      {
        flywheel->enable();
        intake->setState(IntakeController::shootIndexer);
        if(intake->getState() != IntakeController::shootIndexer) completeJob();
        break;
      }

      case shootBoth:
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
