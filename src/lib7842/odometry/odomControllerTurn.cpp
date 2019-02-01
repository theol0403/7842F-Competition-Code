#include "odomController.hpp"

namespace lib7842
{
  using namespace OdomMath;


  void OdomController::turnToAngleSettle(QAngle angle, std::function<bool(OdomController*)> settleFunction)
  {
    angle = rollAngle180(angle);
    turnPid->reset();
    do {
      QAngle angleErr = rollAngle180(angle - chassis->state.theta);
      double turnVel = chassis->model->maxVelocity * turnPid->calculateErr(angleErr.convert(degree));
      chassis->model->rotate(turnVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->rotate(0);
  }

  void OdomController::turnToAngle(QAngle angle, bool settle)
  {
    if(settle) {
      turnToAngleSettle(angle, turnSettle);
    } else {
      turnToAngleSettle(angle, turnNoSettle);
    }
  }

  void OdomController::turnAngle(QAngle angle, bool settle)
  {
    if(settle) {
      turnToAngleSettle(angle + chassis->state.theta, turnSettle);
    } else {
      turnToAngleSettle(angle + chassis->state.theta, turnNoSettle);
    }
  }


  void OdomController::turnToPointSettle(qPoint point, std::function<bool(OdomController*)> settleFunction)
  {
    turnPid->reset();
    do
    {
      double turnVel = chassis->model->maxVelocity * turnPid->calculateErr(computeAngleToPoint(point).convert(degree));
      chassis->model->rotate(turnVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->rotate(0);
  }

  void OdomController::turnToPoint(qPoint point, bool settle)
  {
    if(settle) {
      turnToPointSettle(point, turnSettle);
    } else {
      turnToPointSettle(point, turnNoSettle);
    }
  }

}
