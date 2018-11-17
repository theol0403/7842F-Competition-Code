#include "main.h"
#pragma once
#include "Include/Shared/MotorConfig.hpp"


enum intakeModes_t
{
  intakeCollect,
  intakeShoot,
  intakeOff,
  intakeOut,
  indexShoot
};

void setIntakeMode(intakeModes_t);