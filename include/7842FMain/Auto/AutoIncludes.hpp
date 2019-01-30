#pragma once
#include "main.h"

#include "7842FMain/MotorConfig.hpp"
#include "lib7842/other/miscUtils.hpp"
#include "lib7842/auton/autonSelector.hpp"

#include "7842FMain/Shared/IntakeTask.hpp"
#include "7842FMain/Shared/FlywheelTask.hpp"

void AutoNothing(autonSides);
void AutoTest(autonSides);
void AutoClose(autonSides);
void AutoFar(autonSides);
void AutoMiddle(autonSides);


#define FX 12_ft- //Flips X
#define FA - //Flips Angle


extern qPoint leftFlag;
extern qPoint rightFlag;
