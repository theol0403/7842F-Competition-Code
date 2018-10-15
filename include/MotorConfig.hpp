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




void setFlywheelPower(int flywheelSpeed);

void setIntakePower(int intakeSpeed);

void setIndexerPower(int indexerSpeed);

void setBasePower(int leftSpeed, int rightSpeed);

void setArmPower(int armSpeed);
