#include "controller.hpp"

namespace lib7842
{

void OdomController::turnAngleOkapi(QAngle angle)
{
  okapiChassis->turnAngle(angle);
}
void OdomController::turnToAngleOkapi(QAngle angle)
{
  okapiChassis->turnAngle(rollAngle180(angle - tracker->state.theta));
}
void OdomController::turnToPointOkapi(qPoint point)
{
  okapiChassis->turnAngle(computeAngleToPoint(point));
}

void OdomController::driveDistanceOkapi(QLength distance)
{
  okapiChassis->moveDistanceAsync(distance);
}

void OdomController::driveToPointOkapi(qPoint point)
{
  int direction = 1;
  QAngle wantedAngle = computeAngleToPoint(point);
  if (wantedAngle.abs() > 90_deg)
  {
    wantedAngle -= 180_deg * sgn(wantedAngle.convert(degree));
    direction = -1;
  }
  okapiChassis->turnAngle(wantedAngle);

  okapiChassis->moveDistance(computeDistanceToPoint(point) * direction);
}

} // namespace lib7842
