#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  chassis->driveToPoint({1.5_tl, 0_ft}, 1, true);
  tracker->getY() += 5_in;
  tracker->setState({tracker->getState(), 5_deg});

  Path myPath ({{{{0,0}},0_in, 0_in}, {0_in, 0_in}});
  myPath.add({0_in, 0_in});
  myPath.add({{0_in, 0_in}, {0_in, 0_in}});
}
