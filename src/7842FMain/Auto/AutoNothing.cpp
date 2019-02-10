#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  // chassis->driveDistance(1_ft);
  // chassis->driveToPoint({2_ft, 1_ft}, 3, makeSettle(1_in));
  //chassis->driveToPoint({0_in, 1_ft}, 3, driveSettle);
  chassis->drivePath({{0_in, 2_ft}, {2_ft, 2_ft}, {0_ft, 0_ft}}, 4, makeSettle(4_in));


}
