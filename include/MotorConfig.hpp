#include "main.h"


extern pros::Controller j_Main;

extern pros::Motor m_RightBase2;
extern pros::Motor m_RightBase;
extern pros::Motor m_Intake;
extern pros::Motor m_Flywheel;
extern pros::Motor m_Arm;
extern pros::Motor m_LeftBase;
extern pros::Motor m_LeftBase2;
extern pros::Motor m_Indexer;




void setFlywheelPower(int);

void setIntakePower(int);

void setIndexerPower(int);

void setBasePower(int, int);

void setArmPower(int);
