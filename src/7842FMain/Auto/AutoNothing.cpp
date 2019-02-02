#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  chassis->drivePath({{0_in, 0.5_ft}, {0.5_ft, 0.5_ft}, {0.5_ft, 0_ft}, {0_ft, 0_ft}}, 5, makeSettle(1_in));

}
