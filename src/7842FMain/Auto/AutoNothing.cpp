#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  //chassis->turnAngle(90_deg, false, rightPivot);
  //chassis->driveToPoint({0_in, 1_ft});
  chassis->drivePath({{0_in, 2_ft}, {2_ft, 2_ft}}, 3);//, {2_ft, 0_ft}, {0_ft, 0_ft}}, 10);// makeSettle(1_in));


}
