#pragma once
#include "main.h"
#include "7842FMain/Shared/MotorConfig.hpp"

enum class intakeModes
{
  loading,
  collecting,
  off,
  shoot
};

void intakeControlTask(void*);

extern bool runIntake;

extern intakeModes intakeMode;
