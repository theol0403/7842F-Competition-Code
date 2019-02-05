#include "controller.hpp"

namespace lib7842
{

  void OdomController::driveToPoint(qPoint targetPoint, double turnScale, settleFunc_t settleFunction)
  {
    do
    {
      qPoint closestPoint = closest(chassis->state, targetPoint);

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      if(std::isnan(angleToClose.convert(degree))) angleToClose = 0_deg;

      QLength distanceToClose = computeDistanceToPoint(closestPoint);
      if(angleToClose.abs() >= 90_deg) distanceToClose = -distanceToClose;

      m_angleErr = computeAngleToPoint(targetPoint);
      QLength distanceToTarget = computeDistanceToPoint(targetPoint);

      if(distanceToTarget.abs() < chassis->m_chassisWidth) {
        m_angleErr = 0_deg;
        m_distanceErr = distanceToClose;
      } else {
        m_distanceErr = distanceToTarget;
      }

      if(m_angleErr.abs() > 90_deg)
      {
        m_angleErr += 180_deg;
        m_angleErr = rollAngle180(m_angleErr);// * sgn(m_angleErr.convert(degree));
      }

      double angleVel = anglePid->calculateErr(m_angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = distancePid->calculateErr(distanceToClose.convert(millimeter));

      driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    driveVector(0, 0);
  }



  void OdomController::driveToPointSimple(qPoint targetPoint, double turnScale, settleFunc_t settleFunction)
  {
    distancePid->reset();
    anglePid->reset();
    do
    {
      m_angleErr = computeAngleToPoint(targetPoint);
      m_distanceErr = computeDistanceToPoint(targetPoint);

      if(m_angleErr.abs() > 90_deg)
      {
        m_angleErr += 180_deg;
        m_angleErr = rollAngle180(m_angleErr);
        m_distanceErr = -m_distanceErr;
      }

      double angleVel = anglePid->calculateErr(m_angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = distancePid->calculateErr(m_distanceErr.convert(millimeter));

      driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(m_distanceErr > chassis->m_chassisWidth);

    driveDistanceAtAngle(m_distanceErr / 2, chassis->state.theta, turnScale, settleFunction);
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
