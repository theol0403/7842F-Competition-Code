#include "odomController.hpp"

namespace lib7842
{
  using namespace OdomMath;


  void OdomController::driveToPoint1Settle(qPoint targetPoint, double turnScale, std::function<bool(OdomController*)> settleFunction)
  {
    distancePid->reset();
    anglePid->reset();

    do
    {
      QAngle angleErr = computeAngleToPoint(targetPoint);
      QLength distanceToTarget = computeDistanceToPoint(targetPoint);
      if(distanceToTarget.abs() < 3_in) { angleErr = 0_deg; }

      if(angleErr.abs() > 90_deg)
      {
        angleErr -= 180_deg;
        angleErr = rollAngle180(angleErr) * sgn(angleErr.convert(degree));
      }

      qPoint closestPoint = closest(chassis->state, targetPoint);

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      /* if(angleToClose.abs() < 0.5_deg || std::isnan(angleToClose.convert(degree))) { angleToClose = 0_deg; } */

      QLength distanceErr = computeDistanceToPoint(closestPoint);

      if(angleToClose.abs() >= 90_deg)
      {
        distanceErr = -distanceErr;
      }

      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(distanceErr.convert(millimeter));

      normalizeDrive(distanceVel, angleVel);
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(10);
    }
    while(!settleFunction(this));

    chassis->model->driveVector(0, 0);
  }

  void OdomController::driveToPoint1(qPoint targetPoint, double turnScale, bool settle)
  {
    if(settle) {
      driveToPoint1Settle(targetPoint, turnScale, driveSettle);
    } else {
      driveToPoint1Settle(targetPoint, turnScale, driveNoSettle);
    }
  }






  void OdomController::driveToPoint2Settle(qPoint targetPoint, double turnScale, std::function<bool(OdomController*)> settleFunction)
  {
    distancePid->reset();
    anglePid->reset();
    QAngle angleErr;
    QLength distanceErr;
    do
    {
      angleErr = computeAngleToPoint(targetPoint);
      distanceErr = computeDistanceToPoint(targetPoint);

      if(angleErr.abs() > 90_deg)
      {
        angleErr -= 180_deg;
        angleErr = rollAngle180(angleErr) * sgn(angleErr.convert(degree));
        distanceErr = -distanceErr;
      }

      double angleVel = chassis->model->maxVelocity * anglePid->calculateErr(angleErr.convert(degree) / turnScale) * turnScale;
      double distanceVel = chassis->model->maxVelocity * distancePid->calculateErr(distanceErr.convert(millimeter));

      normalizeDrive(distanceVel, angleVel);
      chassis->model->driveVector(distanceVel, angleVel);
      pros::delay(100);
    }
    while(distanceErr.abs() > 4_in);

    driveDistanceAtAngleSettle(distanceErr/2, chassis->state.theta, turnScale, settleFunction, false);

    chassis->model->driveVector(0, 0);
  }

  void OdomController::driveToPoint2(qPoint targetPoint, double turnScale, bool settle)
  {
    if(settle) {
      driveToPoint2Settle(targetPoint, turnScale, driveSettle);
    } else {
      driveToPoint2Settle(targetPoint, turnScale, driveNoSettle);
    }
  }


}
