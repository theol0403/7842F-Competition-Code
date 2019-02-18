#include "sideController.hpp"

namespace lib7842
{

  settleFunc_t SideController::makeSettle(QAngle angle) {
    return OdomController::makeSettle(mirrorSide(angle, side));
  }
  settleFunc_t SideController::makeSettle(QLength length); {
    return OdomController::makeSettle(length);
  }
  settleFunc_t SideController::makeSettle(QLength length, QAngle angle); {
    return OdomController::makeSettle(length, mirrorSide(angle, side));
  }
  bool SideController::turnSettle(OdomController* that); {
    OdomController::turnSettle(that);
  }
  bool SideController::driveSettle(OdomController* that); {
    OdomController::driveSettle(that);
  }

  turnFunc_t SideController::makeArc(double leftRatio, double rightRatio); {
    return side == autonSides::red ? OdomController::makeArc(leftRatio, rightRatio); : OdomController::makeArc(rightRatio, leftRatio);
  }
  void SideController::pointTurn(OdomController* that, double vel); {
    OdomController::pointTurn(that, vel);
  }

  void SideController::leftPivot(OdomController* that, double vel); {
    side == autonSides::red ? OdomController::leftPivot(that, vel) : OdomController::rightPivot(that, vel);
  }
  void SideController::rightPivot(OdomController* that, double vel); {
    side == autonSides::red ? OdomController::rightPivot(that, vel) : OdomController::leftPivot(that, vel);
  }

  angleCalc_t SideController::angleCalc(QAngle angle); {
    return OdomController::angleCalc(mirrorSide(angle, side));
  }
  angleCalc_t SideController::angleCalc(qPoint point); {
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
  void SideController::turnToPoint(qPoint point, turnFunc_t turnFunc, settleFunc_t settleFunc, AsyncActionList actions) {
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

  void SideController::driveToPoint(qPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->driveToPoint(mirrorSide(targetPoint, side), turnScale, settleFunc, actions);
  }
  void SideController::driveToPointSimple(qPoint targetPoint, double turnScale, settleFunc_t settleFunc, AsyncActionList actions) {
    controller->driveToPointSimple(mirrorSide(targetPoint, side), turnScale, settleFunc, actions);
  }

  void SideController::drivePath(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions) {
    controller->drivePath(mirrorSide(path, side), turnScale, moveOnSettle, finalSettle, actions);
  }
  void SideController::drivePathSimple(Path path, double turnScale, settleFunc_t moveOnSettle, settleFunc_t finalSettle, AsyncActionList actions) {
    controller->drivePathSimple(mirrorSide(path, side), turnScale, moveOnSettle, finalSettle, actions);
  }

}
