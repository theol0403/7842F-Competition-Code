#pragma once
#include "main.h"
#include "7842FMain/Shared/MotorConfig.hpp"
#include "lib7842/pid/pidVelSystem.hpp"

extern bool extendArm;

void setFlywheelRPM(int);
void flywheelTask(void*);
