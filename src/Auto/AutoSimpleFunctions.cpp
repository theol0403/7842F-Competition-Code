#include "main.h"
#include "Include/Auto/AutoSimpleFunctions.hpp"

void setIntakeMode(intakeModes_t intakeMode)
{
  if(intakeMode == intakeCollect)
  {
    setIntakePower(-127);
    setIndexerPower(0);
  }
  else if(intakeMode == intakeShoot)
  {
    setIntakePower(-127);
    setIndexerPower(-127);
  }
  else if(intakeMode == intakeOff)
  {
    setIntakePower(0);
    setIndexerPower(0);
  }
  else if(intakeMode == intakeOut)
  {
    setIntakePower(127);
    setIndexerPower(127);
  }
  else if(intakeMode == indexShoot)
  {
    setIndexerPower(-127);
    setIntakePower(0);
  }

}
