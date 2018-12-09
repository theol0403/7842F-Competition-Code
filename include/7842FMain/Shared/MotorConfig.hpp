#pragma once
#include "main.h"

extern okapi::Controller j_Main;



void setFlywheelPower(double);
double getFlywheelRPM();
double getFlywheelEncoder();

void setIntakePower(double);
void setIndexerPower(double);

extern std::shared_ptr<okapi::OdomChassisController> robotChassis;
extern std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile;

void initializeBase();
void checkBaseStatus();


void setBaseArcade(double, double);
void setBasePower(double, double);
