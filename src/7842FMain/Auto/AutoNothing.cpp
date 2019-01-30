#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  tracker->setState({{0, 0, 0}});
  chassis->driveToPointSettle(leftFlag, OdomController::createDriveSettle(1_in));
  chassis->turnToPoint(leftFlag);
  //, static_cast<std::function<bool(lib7842::OdomController*)>>([](lib7842::OdomController* that)
    // {
    //   return that->distancePid->isSettled();
    // }));
}
