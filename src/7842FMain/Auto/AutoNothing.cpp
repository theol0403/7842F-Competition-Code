#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  chassis->driveDistance(1_ft, false);
  chassis->turnToAngle(90_deg, false);
  chassis->turnToAngle(0_deg, false);
  chassis->driveDistanceAtAngle(-1_ft, 0_deg, true);
  //chassis->turnToAngle(90_deg, false);
  //chassis->driveToPoint(lib7842::Point{0_ft, 1_ft});
}
