#include "controller.hpp"

namespace lib7842
{

  void OdomController::driveToPoint(qPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions)
  {
    resetEmergencyAbort();
    QAngle lastTarget = tracker->state.theta;
    do
    {
      qPoint closestPoint = closest(tracker->state, targetPoint);

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      if(std::isnan(angleToClose.convert(degree))) angleToClose = 0_deg;

      QLength distanceToClose = computeDistanceToPoint(closestPoint);
      if(angleToClose.abs() >= 90_deg) distanceToClose = -distanceToClose;

      m_angleErr = computeAngleToPoint(targetPoint);

      QLength distanceToTarget = computeDistanceToPoint(targetPoint);

      if(distanceToTarget.abs() < 2_ft) {
        m_angleErr = lastTarget - tracker->state.theta;
        m_distanceErr = distanceToClose;
      } else {
        m_angleErr = computeAngleToPoint(targetPoint);
        lastTarget = m_angleErr + tracker->state.theta;
        m_distanceErr = distanceToTarget;
      }

      m_angleErr = rollAngle90(m_angleErr);

      double angleVel = anglePid->step(-m_angleErr.convert(degree));
      double distanceVel = distancePid->step(-distanceToClose.convert(millimeter));

      driveVector(distanceVel, angleVel * turnScale);
      runActions(actions);
      pros::delay(10);
    }
    while(!settleFunc(this));

    driveVector(0, 0);
  }



  void OdomController::driveToPointSimple(qPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions)
  {
    resetEmergencyAbort();
    settleFunc_t exitFunc = makeSettle(2_in);
    distancePid->reset();
    anglePid->reset();
    do
    {
      m_angleErr = computeAngleToPoint(targetPoint);
      m_distanceErr = computeDistanceToPoint(targetPoint);

      if(m_angleErr.abs() > 90_deg)
      { m_distanceErr = -m_distanceErr; }
      m_angleErr = rollAngle90(m_angleErr);

      double angleVel = anglePid->step(-m_angleErr.convert(degree));
      double distanceVel = distancePid->step(-m_distanceErr.convert(millimeter));

      driveVector(distanceVel, angleVel * turnScale);
      runActions(actions);
      pros::delay(10);
    }
    while(!(exitFunc(this) || settleFunc(this)));

    driveDistanceAtAngle(m_distanceErr / 2, angleCalc(tracker->state.theta), turnScale, settleFunc, actions);
    driveVector(0, 0);
  }



  void OdomController::drivePath(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions)
  {
    for(qPoint &point : path.wayPoints)
    {
      driveToPoint(point, turnScale, moveOnSettle, actions);
    }
    driveToPoint(path.wayPoints.back(), turnScale, finalSettle, actions);
  }

  void OdomController::drivePathSimple(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions)
  {
    for(qPoint &point : path.wayPoints)
    {
      driveToPointSimple(point, turnScale, moveOnSettle, actions);
    }
    driveToPointSimple(path.wayPoints.back(), turnScale, finalSettle, actions);
  }


}
