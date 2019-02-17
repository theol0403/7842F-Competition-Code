#pragma once
#include "main.h"
#include "lib7842/lib7842.hpp"
#include "7842FMain/Auto/AutoIncludes.hpp"

extern okapi::Controller j_Main;
#define j_Digital(x) j_Main.getDigital(okapi::ControllerDigital::x)
#define j_Analog(x) j_Main.getAnalog(okapi::ControllerAnalog::x)

void setFlywheelPower(double);
double getFlywheelRPM();
double getFlywheelEncoder();

void setIntakePower(double);
void setIndexerPower(double);
void setIntakeVelocity(double);
void setIndexerVelocity(double);

double getIndexerSensor();

void initializeDevices();

extern std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> model;
extern std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile;

extern lib7842::OdomController* chassis;
extern lib7842::OdomTracker* tracker;

void initializeBase();
void checkBaseStatus();


void setBaseArcade(double, double);
void setBasePower(double, double);

extern okapi::ADIEncoder* s_leftEncoder;
extern okapi::ADIEncoder* s_rightEncoder;
extern okapi::ADIEncoder* s_middleEncoder;

extern lib7842::AutonSelector *autonSelector;

#define TEST_ROBOT
