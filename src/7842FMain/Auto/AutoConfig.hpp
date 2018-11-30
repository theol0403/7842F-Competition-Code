#pragma once
#include "main.h"
#include "7842FMain/Shared/MotorConfig.hpp"

//extern std::shared_ptr<ChassisControllerPID> robotChassis;

enum intakeModes_t
{
  intakeCollect,
  intakeShoot,
  intakeOff,
  intakeOut,
  indexShoot
};

void setIntakeMode(intakeModes_t);
