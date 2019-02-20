#include "ShootController.hpp"

ShootController::ShootController(lib7842::OdomTracker* itracker, IntakeController* iintake, FlywheelController* iflywheel) :
tracker(itracker), intake(iintake), flywheel(iflywheel),
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

void ShootController::run()
{

  while(true)
  {

    switch(getCurrentJob())
    {
      case standby:
      {

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
