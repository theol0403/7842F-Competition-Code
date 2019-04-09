#include "controller.hpp"

namespace lib7842
{

  void OdomController::driveToPoint(qPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions)
  {
    resetPid();
    QAngle lastTarget = tracker->state.theta;
    do
    {
      qPoint closestPoint = closest(tracker->state, targetPoint);

      QAngle angleToClose = computeAngleToPoint(closestPoint);
      if(std::isnan(angleToClose.convert(degree))) angleToClose = 0_deg;

      QLength distanceToClose = computeDistanceToPoint(closestPoint);
      if(angleToClose.abs() >= 90_deg) distanceToClose = -distanceToClose;

      angleErr = computeAngleToPoint(targetPoint);

      QLength distanceToTarget = computeDistanceToPoint(targetPoint);

      if(distanceToTarget.abs() < pointRadius) {
        angleErr = 0_deg;
        distanceErr = distanceToClose;
      } else {
        angleErr = computeAngleToPoint(targetPoint);
        lastTarget = angleErr + tracker->state.theta;
        distanceErr = distanceToTarget;
      }

      angleErr = rollAngle90(angleErr);

      double angleVel = anglePid->step(-angleErr.convert(degree));
      double distanceVel = distancePid->step(-distanceToClose.convert(millimeter));

      driveVector(distanceVel, angleVel * turnScale);
      runActions(actions);
      pros::delay(10);
    }
    while(!settleFunc(this));

    driveVector(0, 0);
  }



  void OdomController::driveToPoint2(qPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions)
  {
    resetPid();
    settleFunc_t exitFunc = makeSettle(pointRadius);
    do
    {
      angleErr = computeAngleToPoint(targetPoint);
      distanceErr = computeDistanceToPoint(targetPoint);

      if(angleErr.abs() > 90_deg) distanceErr = -distanceErr;
      angleErr = rollAngle90(angleErr);

      double angleVel = anglePid->step(-angleErr.convert(degree));
      double distanceVel = distancePid->step(-distanceErr.convert(millimeter));

      driveVector(distanceVel, angleVel * turnScale);
      runActions(actions);
      pros::delay(10);
    }
    while(!(exitFunc(this) || settleFunc(this)));

    driveDistanceAtAngle(computeDistanceToPoint(targetPoint), angleCalc(computeAngleToPoint(targetPoint)), turnScale, settleFunc, actions);
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

  void OdomController::drivePath2(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions)
  {
    for(qPoint &point : path.wayPoints)
    {
      driveToPoint2(point, turnScale, moveOnSettle, actions);
    }
    driveToPoint2(path.wayPoints.back(), turnScale, finalSettle, actions);
  }


}
