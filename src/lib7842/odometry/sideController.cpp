#include "sideController.hpp"
#include "lib7842/other/miscUtils.hpp"

namespace lib7842
{

  SideController::SideController(OdomController* icontroller, autonSides iside) : controller(icontroller), side(iside) {}

  AsyncAction SideController::makeAsyncAction() {
    return AsyncAction().withSide(side);
  }
  
  void SideController::setState(QPoint point) {
    controller->tracker->setState(mirrorSide(point, side));
  }

  void SideController::setX(QLength x) {
    controller->tracker->setX(mirrorSide(x, side));
  }

  QAngle SideController::angleToPoint(QPoint point) {
    return controller->angleToPoint(mirrorSide(point, side));
  }
  QLength SideController::distanceToPoint(QPoint point) {
    return controller->distanceToPoint(mirrorSide(point, side));
  }


  settleFunc_t SideController::makeSettle(QAngle angle) {
    return OdomController::makeSettle(mirrorSide(angle, side));
  }
  settleFunc_t SideController::makeSettle(QLength length) {
    return OdomController::makeSettle(length);
  }
  settleFunc_t SideController::makeSettle(QLength length, QAngle angle) {
    return OdomController::makeSettle(length, mirrorSide(angle, side));
  }

  turnFunc_t SideController::makeArc(double leftRatio, double rightRatio) {
    return side == autonSides::red ? OdomController::makeArc(leftRatio, rightRatio) : OdomController::makeArc(rightRatio, leftRatio);
  }
  void SideController::leftPivot(OdomController* that, double vel) {
    side == autonSides::red ? OdomController::leftPivot(that, vel) : OdomController::rightPivot(that, vel);
  }
  void SideController::rightPivot(OdomController* that, double vel) {
    side == autonSides::red ? OdomController::rightPivot(that, vel) : OdomController::leftPivot(that, vel);
  }

  angleCalc_t SideController::angleCalc(QAngle angle) {
    return OdomController::angleCalc(mirrorSide(angle, side));
  }
  angleCalc_t SideController::angleCalc(QPoint point) {
    return OdomController::angleCalc(mirrorSide(point, side));
  }


  void SideController::turn(angleCalc_t turnCalc, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->turn(turnCalc, turnFunc, settleFunc, actions);
  }
  void SideController::turnToAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->turnToAngle(mirrorSide(angle, side), turnFunc, settleFunc, actions);
  }
  void SideController::turnAngle(QAngle angle, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->turnAngle(mirrorSide(angle, side), turnFunc, settleFunc, actions);
  }
  void SideController::turnToPoint(QPoint point, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->turnToPoint(mirrorSide(point, side), turnFunc, settleFunc, actions);
  }

  void SideController::driveDistanceAtAngle(QLength distance, angleCalc_t turnCalc, double turnScale, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->driveDistanceAtAngle(distance, turnCalc, turnScale, settleFunc, actions);
  }
  void SideController::driveDistance(QLength distance, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->driveDistance(distance, settleFunc, actions);
  }
  void SideController::driveForTime(int time, double vel, AsyncActionList actions) {
    controller->driveForTime(time, vel, actions);
  }
  void SideController::driveForTimeAtAngle(int time, double vel, angleCalc_t turnCalc, double turnScale, AsyncActionList actions) {
    controller->driveForTimeAtAngle(time, vel, turnCalc, turnScale, actions);
  }
  void SideController::allignToAngle(QAngle angle, double vel, double velThresh) {
    controller->allignToAngle(mirrorSide(angle, side), vel, velThresh);
  }

  void SideController::driveToPoint(QPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->driveToPoint(mirrorSide(targetPoint, side), turnScale, settleFunc, actions);
  }
  void SideController::driveToPoint2(QPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->driveToPoint2(mirrorSide(targetPoint, side), turnScale, settleFunc, actions);
  }

  void SideController::drivePath(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions) {
    controller->drivePath(mirrorSide(path, side), turnScale, moveOnSettle, finalSettle, actions);
  }
  void SideController::drivePath2(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions) {
    controller->drivePath2(mirrorSide(path, side), turnScale, moveOnSettle, finalSettle, actions);
  }

  void SideController::driveToPointSimple(QPoint point, settleFunc_t settleFunc, AsyncActionList actions)
  {
    int direction = 1;
    QAngle wantedAngle = angleToPoint(point);
    if (wantedAngle.abs() > 90_deg)
    {
      wantedAngle -= 180_deg * sgn(wantedAngle.convert(degree));
      direction = -1;
    }
    controller->turnAngle(wantedAngle);

    controller->driveDistance(distanceToPoint(point) * direction, settleFunc, actions);
  }

}
