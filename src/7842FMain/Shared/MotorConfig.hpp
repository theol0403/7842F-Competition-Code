#pragma once
#include "main.h"

extern const int8_t e_RightFront;
extern const int8_t e_RightBack;
extern const int8_t e_LeftFront;
extern const int8_t e_LeftBack;
extern const int8_t e_Flywheel;
extern const int8_t e_Flywheel2;
extern const int8_t e_Intake;
extern const int8_t e_Indexer;

extern Controller j_Main;

void setFlywheelPower(int);
int getFlywheelRPM();
int getFlywheelEncoder();

void setIntakePower(int);
void setIndexerPower(int);

extern std::shared_ptr<OdomChassisController> robotChassis;
void setBaseArcade(float, float);
void setBasePower(float, float);
