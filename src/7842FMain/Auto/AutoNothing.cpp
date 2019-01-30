#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  chassis->driveToPointSettle({0_ft, 1_ft}, OdomController::createDriveSettle(1_in)); 
  //, static_cast<std::function<bool(lib7842::OdomController*)>>([](lib7842::OdomController* that)
    // {
    //   return that->distancePid->isSettled();
    // }));
}
