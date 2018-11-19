#pragma once
#include "main.h"


extern const int8_t e_RightBase;
extern const int8_t e_RightBase2;
extern const int8_t e_LeftBase;
extern const int8_t e_LeftBase2;
extern const int8_t e_Flywheel;
extern const int8_t e_Flywheel2;
extern const int8_t e_Intake;
extern const int8_t e_Indexer;


extern pros::Controller j_Main;


void setBasePower(int, int);
double getBaseRight();
double getBaseLeft();


void setFlywheelPower(int);
int getFlywheelRPM();
int getFlywheelEncoder();

void setIntakePower(int);
void setIndexerPower(int);
