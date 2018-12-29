#pragma once
#include "main.h"
#include "7842FMain/Shared/MotorConfig.hpp"

enum class intakeModes
{
  off,
  loading, //Ball into indexer
  collecting, //Intake running
  shootBoth,
  shootIndexer,
  out
};

void intakeControlTask(void*);

void setIntakeMode(intakeModes);
