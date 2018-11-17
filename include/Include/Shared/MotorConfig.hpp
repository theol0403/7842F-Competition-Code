#include "main.h"
#pragma once

	const int e_RightBase = -5;
	const int e_RightBase2 = -6;
	const int e_LeftBase = 7;
	const int e_LeftBase2 = 8;
	const int e_Flywheel = 11;
	const int e_Flywheel2 = -12;
	const int e_Intake = 1;
	const int e_Indexer = 2;


extern pros::Controller j_Main;


void setBasePower(int, int);
double getBaseRight();
double getBaseLeft();


void setFlywheelPower(int);
int getFlywheelRPM();
int getFlywheelEncoder();

void setIntakePower(int);
void setIndexerPower(int);
