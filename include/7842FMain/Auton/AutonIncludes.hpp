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
void AutonTest(void*);

void AutonClose(void*);
void AutonSkills(void*);

void AutonFar(void*);
void AutonFarDescore(void*);
void AutonFarPost(void*);

void AutonMiddleFromClose(void*);
void AutonMiddleFromFar(void*);


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


/**
* Actions
*/
void firstCapMovement(SideController& chassis, QLength y);
void waitForLastMinute(Timer& timer);
