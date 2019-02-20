#include "ShootController.hpp"

ShootController::ShootController(lib7842::OdomTracker* itracker, IntakeController* iintake, FlywheelController* iflywheel, pros::ADIPotentiometer* ihoodSensor) :
tracker(itracker), intake(iintake), flywheel(iflywheel), hoodSensor(ihoodSensor),
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

void ShootController::run()
{
  const double standbyAngle = 200;
  const double angleThresh = 30;

  while(true)
  {

    switch(getCurrentJob())
    {
      case standby:
      {
        if(getAngle() > standbyAngle + angleThresh) {
          addJob(cycle);
        } else {
          flywheel->enable();
        }
        break;
      }

      case angleTop:
      {
        flywheel->disable();
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
