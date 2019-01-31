#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  tracker->setState({});
  chassis->driveToPointSettle(leftFlag, OdomController::createDriveSettle(1_in));
  chassis->turnToPoint(leftFlag);
}
