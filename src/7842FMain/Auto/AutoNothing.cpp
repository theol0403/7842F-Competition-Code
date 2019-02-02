#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  chassis->driveToPoint({1.5_tl, 0_ft}, 5, OdomController::makeSettle(1_in, 2_deg));
  tracker->getY() += 5_in;
  tracker->setState({tracker->getState(), 5_deg});

}
