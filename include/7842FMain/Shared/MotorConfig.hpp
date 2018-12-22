#pragma once
#include "main.h"

extern okapi::Controller j_Main;

void setFlywheelPower(double);
double getFlywheelRPM();
double getFlywheelEncoder();

void setIntakePower(double);
void setIndexerPower(double);
void setIndexerVelocity(double);

double getIndexerSensor();

void initializeDevices();

extern std::shared_ptr<okapi::OdomChassisController> robotChassis;
extern std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile;

void initializeBase();
void checkBaseStatus();


void setBaseArcade(double, double);
void setBasePower(double, double);



extern okapi::ADIEncoder* s_leftEncoder;
extern okapi::ADIEncoder* s_rightEncoder;
extern okapi::ADIEncoder* s_middleEncoder;
