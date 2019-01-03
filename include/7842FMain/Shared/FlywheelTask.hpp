#pragma once
#include "main.h"
#include "7842FMain/MotorConfig.hpp"
#include "lib7842/pid/pidVelSystem.hpp"

void flywheelTask(void*);

void setFlywheelRPM(int);
void setFlywheelArmMode(bool);
