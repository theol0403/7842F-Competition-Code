#include "odomController.hpp"

namespace lib7842
{

  void OdomController::turnToAngleSettle(QAngle angle, std::function<bool(OdomController*)> settleFunction)
  {
    angle = rollAngle180(angle);
    turnPid->reset();
    do {
      m_angleErr = rollAngle180(angle - chassis->state.theta);
      double turnVel = chassis->model->maxVelocity * turnPid->calculateErr(m_angleErr.convert(degree));
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
      m_angleErr = computeAngleToPoint(point);
      double turnVel = chassis->model->maxVelocity * turnPid->calculateErr(m_angleErr.convert(degree));
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
