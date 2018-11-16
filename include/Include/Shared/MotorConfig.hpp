#include "main.h"
#pragma once

enum motorConfig_t
{
	e_RightBase = -15,
	e_RightBase2 = -16,
	e_LeftBase = 17,
	e_LeftBase2 = 18,
	e_Flywheel = 13,
	e_Flywheel2 = -14,
	e_Intake = 11,
	e_Indexer = 12
};

extern pros::Controller j_Main;


void setBasePower(int, int);
double getBaseRight();
double getBaseLeft();


void setFlywheelPower(int);
int getFlywheelRPM();
int getFlywheelEncoder();

void setIntakePower(int);
void setIndexerPower(int);
