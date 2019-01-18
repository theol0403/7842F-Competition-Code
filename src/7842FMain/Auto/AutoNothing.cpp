#include "7842FMain/Auto/AutoIncludes.hpp"

void AutoNothing(lib7842::autonSides side)
{

}

void AutoTest(lib7842::autonSides side)
{
  chassisOdomController->driveToPoint(lib7842::Point{0_ft, 1_ft});
}


void UnlockDriver()
{
    chassisOdomController->m_chassisController->moveDistance(1_cm);
}
