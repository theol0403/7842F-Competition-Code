#pragma once
#include "main.h"

#define USE_SIDE_MACROS
#include "7842FMain/MotorConfig.hpp"
#include "lib7842/other/miscUtils.hpp"
#include "lib7842/auton/autonSelector.hpp"

#include "7842FMain/Shared/IntakeTask.hpp"
#include "7842FMain/Shared/FlywheelTask.hpp"

/**
* Points
*/
extern qPoint leftFlag;
extern qPoint rightFlag;

/**
* Functions
*/
void AutoNothing(autonSides);
void AutoTest(autonSides);
void AutoClose(autonSides);
void AutoFar(autonSides);
void AutoMiddle(autonSides);
