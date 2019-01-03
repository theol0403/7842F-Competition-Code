#pragma once
#include "main.h"
#include "7842FMain/MotorConfig.hpp"


enum intakeModes_t
{
  intakeCollect,
  intakeShoot,
  intakeOff,
  intakeOut,
  indexShoot
};

void setIntakeMode(intakeModes_t);
