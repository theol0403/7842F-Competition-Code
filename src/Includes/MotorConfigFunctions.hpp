#include "main.h"

extern Motor m_RightBase2;
extern Motor m_RightBase;
extern Motor m_Intake;
extern Motor m_Flywheel;
extern Motor m_Arm;
extern Motor m_LeftBase;
extern Motor m_LeftBase2;
extern Motor m_Indexer;




void setFlywheelPower(int flywheelSpeed);

void setIntakePower(int intakeSpeed);

void setIndexerPower(int indexerSpeed);

void setBasePower(int leftSpeed, int rightSpeed);

void setArmPower(int armSpeed);
