#include "ShootController.hpp"

ShootController::ShootController(IntakeController*& iintake, FlywheelController*& iflywheel, pros::ADIPotentiometer* ihoodSensor, double ibackAngle, IterativePosPIDController* ihoodPid) :
intake(iintake), flywheel(iflywheel), hoodSensor(ihoodSensor), backAngle(ibackAngle), hoodPid(ihoodPid),
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
    addJobs({reportDone, shootIndexer, angleMiddle, shootIndexer, angleTop});
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
  while(!macroCompleted) pros::delay(20);
}


double ShootController::getHoodAngle() {
  return (hoodSensor->get_value() / 4095.0 * 265.0) - backAngle;
}

double ShootController::getTopFlagAngle() {
  double x = distanceToFlag.convert(foot);
  double y = -0.0974*std::pow(x, 2) + 1.8549*x - 1.9294;
  if(x < 3.2) y = 0;
  if(y < 0 || y > 40) y = 0;
  return y;
}

double ShootController::getMiddleFlagAngle() {
  double x = distanceToFlag.convert(foot);
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

double ShootController::computeHoodPower(double target) {
  hoodPid->setTarget(target);
  double output = hoodPid->step(getHoodAngle()) * 127;
  if(output < 0) {
    output = 0;
    std::cerr << "Reverse PID \n";
  }
  //if(output > 100) output = 100;
  if(output < 40) output = 40;
  return output;
}

void ShootController::run()
{
  const double angleThresh = 3;
  const double cycleVel = -50;

  const double extendPos = 46;

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
      if(getHoodAngle() < 5) {
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


      case waitForBall:
      flywheel->enable();
      intake->enable();
      intake->indexerSlave = false;
      if(intake->hasBall) {
        completeJob();
      } else {
        intake->setState(IntakeController::intakeBall);
      }
      break;


      case waitForFlywheel:
      flywheel->enable();
      intake->disable();
      intake->indexerSlave = true;
      if(flywheel->pid->getError() < 300) completeJob();
      break;


      case shootIndexer:
      flywheel->enable();
      intake->disable();
      pros::delay(10);
      intake->indexerSlave = false;
      intake->intake->moveVelocity(200);
      intake->indexer->moveVelocity(200);
      pros::delay(250);
      intake->enable();
      intake->setState(IntakeController::intakeBall);
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
    pros::delay(1);
  }

}


void ShootController::task(void* input)
{
  pros::delay(500);
  ShootController* that = static_cast<ShootController*>(input);
  that->run();
}
