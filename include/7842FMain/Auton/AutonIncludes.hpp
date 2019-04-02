#pragma once
#include "main.h"

#define CONTROLLER_NAME_FOR_SHORTCUTS chassis->

#include "7842FMain/RobotConfig.hpp"
#include "lib7842/auton/sideController.hpp"
#include "lib7842/other/miscUtils.hpp"

#include "AutonPoints.hpp"

/**
* Functions
*/
void AutonNothing(void*);

void AutonClose(void*);
void AutonCloseWithoutPush(void*);
void AutonCloseExperimental(void*);
void AutonFar(void*);
void AutonCloseMiddle(void*);

void AutonMiddleFromClose(void*);
void AutonMiddleFromFar(void*);

void AutonPlatformFar(void*);

void AutonTest(void*);
