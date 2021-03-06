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
void AutonNothing(void*); // Nothing happens
void AutonTest(void*); // Tests

void AutonClose_NearPark(void*);
void AutonClose_NearMiddle(void*); // good
void AutonClose_NearCross(void*);
void AutonClose_MiddlePark(void*);
void AutonClose_MiddleNear(void*);

void AutonFar_NearPark(void*);
void AutonFar_MiddlePark(void*);
void AutonFar_CrossPark(void*);

void AutonFar_CapNear(void*);
void AutonFar_CapMiddle(void*);
void AutonFar_CapCross(void*);

void AutonSkills(void*);

/**
 * Points
 */
extern QLength startX;

extern QPoint sideFlagShoot;
extern QPoint sideFlagPost;

extern QPoint middleFlagShoot;
extern QPoint middleFlagPost;

extern QPoint farFlagShoot;
extern QPoint farFlagPost;

extern QPoint closeFlatCap;
extern QPoint farFlatCap;

extern QPoint farPost;

/**
 * Actions
 */
void firstCapMovement(SideController& chassis, QLength y);
void waitForLastMinute(Timer& timer);
void closeShootFlagsPush(SideController& chassis);
