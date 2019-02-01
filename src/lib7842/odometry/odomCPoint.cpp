#include "odomController.hpp"

namespace lib7842
{

  void OdomController::driveToPointSettle(qPoint targetPoint, double turnScale, std::function<bool(OdomController*)> settleFunction)
  {
    distancePid->reset();
    anglePid->reset();

    do
    {
      m_m_angleErr = computeAngleToPoint(targetPoint);
      QLength distanceToTarget = computeDistanceToPoint(targetPoint);
      if(distanceToTarget.abs() < 3_in) { m_angleErr = 0_deg; }

      if(m_angleErr.abs() > 90_deg)
      {
        m_angleErr -= 180_deg;
        m_angleErr = rollAngle180(m_angleErr) * sgn(m_angleErr.convert(degree));
      }

      qPoint closestPoint = closest(chassis->state, targetPoint);

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      if(std::isnan(angleToClose.convert(degree))) angleToClose = 0_deg; //check

      m_distanceErr = computeDistanceToPoint(closestPoint);

      if(angleToClose.abs() >= 90_deg)
      {
        m_distanceErr = -m_distanceErr;
      }

      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(m_angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(m_distanceErr.convert(millimeter));

      normalizeDrive(distanceVel, angleVel);
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->driveVector(0, 0);
  }


  void OdomController::driveToPoint(qPoint targetPoint, double turnScale, bool settle)
  {
    if(settle) {
      driveToPointSettle(targetPoint, turnScale, driveSettle);
    } else {
      driveToPointSettle(targetPoint, turnScale, driveNoSettle);
    }
  }




  void OdomController::driveToPointSimpleSettle(qPoint targetPoint, double turnScale, std::function<bool(OdomController*)> settleFunction)
  {
    distancePid->reset();
    anglePid->reset();
    do
    {
      m_angleErr = computeAngleToPoint(targetPoint);
      m_distanceErr = computeDistanceToPoint(targetPoint);

      if(m_angleErr.abs() > 90_deg)
      {
        m_angleErr -= 180_deg;
        m_angleErr = rollAngle180(m_angleErr) * sgn(m_angleErr.convert(degree));
        m_distanceErr = -m_distanceErr;
      }

      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(m_angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(m_distanceErr.convert(millimeter));

      normalizeDrive(distanceVel, angleVel);
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(100);
    }
    while(m_distanceErr.abs() > 4_in);

    driveDistanceAtAngleSettle(m_distanceErr / 2, chassis->state.theta, turnScale, settleFunction, false);

    chassis->model->driveVector(0, 0);
  }


  void OdomController::driveToPointSimple(qPoint targetPoint, double turnScale, bool settle)
  {
    if(settle) {
      driveToPointSimpleSettle(targetPoint, turnScale, driveSettle);
    } else {
      driveToPointSimpleSettle(targetPoint, turnScale, driveNoSettle);
    }
  }

  bool OdomController::pathSettle(OdomController* that) {
    return that->distancePid->isSettled() && that->anglePid->isSettled();
  }

  void OdomController::drivePath(Path path, double turnScale, std::function<bool(OdomController*)> settleFunction)
  {
    for(Point &point : path.wayPoints)
    {
      driveToPointSettle(point, turnScale, settleFunction);
    }
  }



}
