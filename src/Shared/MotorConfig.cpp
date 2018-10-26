#include "main.h"
#include "MotorConfig.hpp"




pros::Controller j_Main(pros::E_CONTROLLER_MASTER);


pros::Motor m_RightBase2(4, true);
pros::Motor m_RightBase(10, true);
pros::Motor m_Intake(1, true);
pros::Motor m_Flywheel(2);
pros::Motor m_Arm(3);
pros::Motor m_LeftBase(5);
pros::Motor m_LeftBase2(20);
pros::Motor m_Indexer(9, true);




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
