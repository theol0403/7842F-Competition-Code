#include "controller.hpp"

namespace lib7842
{

  void OdomController::driveToPoint(qPoint targetPoint, double turnScale, settleFunc_t settleFunc)
  {
    resetEmergencyAbort();
    QAngle lastTarget = chassis->state.theta;
    do
    {
      qPoint closestPoint = closest(chassis->state, targetPoint);

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      if(std::isnan(angleToClose.convert(degree))) angleToClose = 0_deg;

      QLength distanceToClose = computeDistanceToPoint(closestPoint);
      if(angleToClose.abs() >= 90_deg) distanceToClose = -distanceToClose;

      m_angleErr = computeAngleToPoint(targetPoint);

      QLength distanceToTarget = computeDistanceToPoint(targetPoint);

      if(distanceToTarget.abs() < chassis->m_chassisWidth && distanceToClose.abs() < chassis->m_chassisWidth / 2) {
        m_angleErr = lastTarget - chassis->state.theta;
        m_distanceErr = distanceToClose;
      } else {
        m_angleErr = computeAngleToPoint(targetPoint);
        lastTarget = m_angleErr + chassis->state.theta;
        m_distanceErr = distanceToTarget;
      }

      m_angleErr = rollAngle90(m_angleErr);

      double angleVel = anglePid->calculateErr(m_angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = distancePid->calculateErr(distanceToClose.convert(millimeter));

      driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!settleFunc(this));

    driveVector(0, 0);
  }



  void OdomController::driveToPointSimple(qPoint targetPoint, double turnScale, settleFunc_t settleFunc)
  {
    resetEmergencyAbort();
    settleFunc_t exitFunc = makeSettle(chassis->m_chassisWidth);
    distancePid->reset();
    anglePid->reset();
    do
    {
      m_angleErr = computeAngleToPoint(targetPoint);
      m_distanceErr = computeDistanceToPoint(targetPoint);

      if(m_angleErr.abs() > 90_deg)
      { m_distanceErr = -m_distanceErr; }
      m_angleErr = rollAngle90(m_angleErr);

      double angleVel = anglePid->calculateErr(m_angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = distancePid->calculateErr(m_distanceErr.convert(millimeter));

      driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!exitFunc);

    driveDistanceAtAngle(m_distanceErr / 2, chassis->state.theta, turnScale, settleFunc);
    driveVector(0, 0);
  }



  void OdomController::drivePath(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle)
  {
    for(qPoint &point : path.wayPoints)
    {
      driveToPoint(point, turnScale, moveOnSettle);
    }
    driveToPoint(path.wayPoints.back(), turnScale, finalSettle);
  }

  void OdomController::drivePathSimple(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle)
  {
    for(qPoint &point : path.wayPoints)
    {
      driveToPointSimple(point, turnScale, moveOnSettle);
    }
    driveToPointSimple(path.wayPoints.back(), turnScale, finalSettle);
  }


}
