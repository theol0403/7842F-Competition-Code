#pragma once
#include "main.h"

#include "7842FMain/MotorConfig.hpp"
#include "lib7842/other/miscUtils.hpp"
#include "lib7842/auton/autonSelector.hpp"

#include "7842FMain/Shared/IntakeTask.hpp"
#include "7842FMain/Shared/FlywheelTask.hpp"

void AutoClose(lib7842::autonSides);
void AutoFar(lib7842::autonSides);
void AutoMiddle(lib7842::autonSides);

#define FX 12_ft- //Flips X
#define FA - //Flips Angle


extern lib7842::Point leftFlag;
extern lib7842::Point rightFlag;
