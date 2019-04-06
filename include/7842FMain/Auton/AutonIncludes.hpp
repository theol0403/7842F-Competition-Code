#pragma once
#include "main.h"

#define CONTROLLER_NAME_FOR_SHORTCUTS chassis.

#include "7842FMain/RobotConfig.hpp"
#include "lib7842/auton/sideController.hpp"
#include "lib7842/other/miscUtils.hpp"

using AutonPasser = std::tuple<SideController, Timer>;

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

void AutonSkills(void*);

/**
 * Points
 */
extern QTime shootTime;
extern QLength startX;

extern qPoint sideFlagShoot;
extern qPoint sideFlagPost;

extern qPoint middleFlagShoot;
extern qPoint middleFlagPost;

extern qPoint closeFlatCap;

extern qPoint sidePlatform;


/**
 * Actions
 */
void firstCapMovement(SideController& chassis, QLength y);
void waitForLastMinute(Timer& timer);
