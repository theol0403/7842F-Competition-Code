#pragma once
#include "main.h"

extern Controller j_Main;

extern const int8_t e_Flywheel;
extern const int8_t e_Flywheel2;
extern const int8_t e_Intake;
extern const int8_t e_Indexer;

extern const int8_t e_RightFront;
extern const int8_t e_RightBack;
extern const int8_t e_LeftFront;
extern const int8_t e_LeftBack;



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
