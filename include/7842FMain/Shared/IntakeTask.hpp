#pragma once
#include "main.h"
#include "7842FMain/MotorConfig.hpp"

enum class intakeModes
{
  off,
  loading, //Ball into indexer
  collecting, //Intake running
  shootIndexer,
  shootBoth,
  outIntake,
  outBoth,
  outSlow
};

void intakeControlTask(void*);

void setIntakeMode(intakeModes);
