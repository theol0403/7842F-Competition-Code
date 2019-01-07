#include "odomController.hpp"

namespace lib7842
{

  OdomController::OdomController
  (
    std::shared_ptr<okapi::ChassisControllerPID> chassisController,
    OdomTracker *odomTracker
  ):
  m_chassisController(chassisController),
  m_odomTracker(odomTracker)
  {
  };


  void OdomController::turnToAngle(QAngle angle)
  {
    IterativePosPIDController angleController ( 1, 0, 0, 0, TimeUtilFactory::create());

    angleController.setTarget(angle.convert(degree));

    while (!angleController.isSettled()) 
    {
    double newOutput = angleController.step(m_odomTracker->state.theta.convert(degree));
    exampleMotor.controllerSet(newOutput);

    pros::delay(10); // Run the control loop at 10ms intervals
  }


    m_chassisController->turnAngle(angle - m_odomTracker->state.theta);
  }

  QLength OdomController::computeDistanceToPoint(Point point)
  {
    const QLength xDiff = point.x - m_odomTracker->state.x;
    const QLength yDiff = point.y - m_odomTracker->state.y;
    return std::sqrt(std::pow(xDiff.convert(inch), 2) + std::pow(yDiff.convert(inch), 2)) * inch;
  }

  QAngle OdomController::computeAngleToPoint(Point point)
  {
    return atan2(point.x.convert(inch) - m_odomTracker->state.x.convert(inch), point.y.convert(inch) - m_odomTracker->state.y.convert(inch)) * radian - m_odomTracker->state.theta;
  }


  void OdomController::turnToAngle(QAngle angle)
  {
    m_chassisController->turnAngle(angle - m_odomTracker->state.theta);
  }

  void OdomController::turnToPoint(Point point)
  {
    m_chassisController->turnAngle(computeAngleToPoint(point));
  }


  void OdomController::driveToPoint(Point point)
  {
    m_chassisController->turnAngle(computeAngleToPoint(point));

    m_chassisController->moveDistance(computeDistanceToPoint(point));
  }


  void OdomController::driveToPointAndAngle(Point point)
  {
    driveToPoint(point);
    turnToAngle(point.theta);
  }


}
