#include "main.h"


extern pros::Controller j_Main;

enum motorConfig_t
{
	e_RightBase = -2,
	e_RightBase2 = -3,
	e_LeftBase = 4,
	e_LeftBase2 = 5,
	e_Flywheel = 7,
	e_Intake = 14,
	e_Indexer = 1
};


void setBasePower(int, int);
double getBaseRight();
double getBaseLeft();


void setFlywheelPower(int);
int getFlywheelRPM();
int getFlywheelEncoder();

void setIntakePower(int);
void setIndexerPower(int);
