#include "controller.hpp"

namespace lib7842
{

  void OdomController::driveToPoint(qPoint targetPoint, double turnScale, settleFunc_t settleFunction)
  {

    do
    {
      m_angleErr = computeAngleToPoint(targetPoint);
      m_distanceErr = computeDistanceToPoint(targetPoint);
      if(m_distanceErr.abs() < chassis->m_chassisWidth) { m_angleErr = 0_deg; }

      if(m_angleErr.abs() > 90_deg)
      {
        m_angleErr += 180_deg;
        m_angleErr = rollAngle180(m_angleErr);// * sgn(m_angleErr.convert(degree));
      }

      qPoint closestPoint = closest(chassis->state, targetPoint);

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      if(std::isnan(angleToClose.convert(degree))) angleToClose = 0_deg; //check

      QLength m_distanceToClose = computeDistanceToPoint(closestPoint);
      if(angleToClose.abs() >= 90_deg) m_distanceToClose = -m_distanceToClose;

      double angleVel = anglePid->calculateErr(m_angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = distancePid->calculateErr(m_distanceToClose.convert(millimeter));

      //normalizeDrive(distanceVel, angleVel);
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->driveVector(0, 0);
  }



  void OdomController::driveToPointSimple(qPoint targetPoint, double turnScale, settleFunc_t settleFunction)
  {
    settleFunc_t exitFunction = driveSettle;//makeSettle(4_in);
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

      normalizeDrive(distanceVel, angleVel);
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!exitFunction(this));

    //driveDistanceAtAngle(m_distanceErr / 2, chassis->state.theta, turnScale, settleFunction, false);

    chassis->model->driveVector(0, 0);
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
