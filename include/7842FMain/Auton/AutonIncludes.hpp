#pragma once
#include "main.h"

#define INSERT_CONTROLLER_NAME chassis.

#include "7842FMain/RobotConfig.hpp"
#include "lib7842/odometry/sideController.hpp"
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

extern QPoint sideFlagShoot;
extern QPoint sideFlagPost;

extern QPoint middleFlagShoot;
extern QPoint middleFlagPost;

extern QPoint closeFlatCap;

extern QPoint sidePlatform;


/**
 * Actions
 */
void firstCapMovement(SideController& chassis, QLength y);
void waitForLastMinute(Timer& timer);
