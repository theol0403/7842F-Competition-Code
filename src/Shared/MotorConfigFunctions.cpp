#include "main.h"





Controller j_Main(E_CONTROLLER_MASTER);


Motor m_RightBase2(2, true);
Motor m_RightBase(3, true);
Motor m_Intake(4, true);
Motor m_Flywheel(5);
Motor m_Arm(6);
Motor m_LeftBase(7);
Motor m_LeftBase2(8);
Motor m_Indexer(9, true);




void setFlywheelPower(int flywheelSpeed)
{
	m_Flywheel.move(flywheelSpeed);
}

void setIntakePower(int intakeSpeed)
{
	m_Intake.move(intakeSpeed); //Positive intakespeed = intake
}


void setIndexerPower(int indexerSpeed)
{
	m_Indexer.move(indexerSpeed);
}

void setBasePower(int leftSpeed, int rightSpeed)
{
	m_RightBase.move(rightSpeed);
	m_LeftBase.move(leftSpeed);
	m_RightBase2.move(rightSpeed);
	m_LeftBase2.move(leftSpeed);
}

void setArmPower(int armSpeed)
{
	m_Arm.move(armSpeed);
}
