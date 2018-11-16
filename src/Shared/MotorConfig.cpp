#include "main.h"
#include "Include/Shared/MotorConfig.hpp"



pros::Controller j_Main(pros::E_CONTROLLER_MASTER);

pros::Motor m_RightBase(abs(e_RightBase), e_RightBase<0);
pros::Motor m_RightBase2(abs(e_RightBase2), e_RightBase2<0);
pros::Motor m_LeftBase(abs(e_LeftBase), e_LeftBase<0);
pros::Motor m_LeftBase2(abs(e_LeftBase2), e_LeftBase2<0);
void setBasePower(int leftPower, int rightPower)
{
	m_RightBase.move(rightPower);
	m_RightBase2.move(rightPower);
	m_LeftBase.move(leftPower);
	m_LeftBase2.move(leftPower);
}
double getBaseRight()
{
	return (m_RightBase.get_position() + m_RightBase2.get_position()) / 2;
}
double getBaseLeft()
{
	return (m_LeftBase.get_position() + m_LeftBase2.get_position()) / 2;
}

pros::Motor m_Flywheel(abs(e_Flywheel), e_Flywheel<0);
pros::Motor m_Flywheel2(abs(e_Flywheel2), e_Flywheel2<0);
void setFlywheelPower(int speed)
{
	m_Flywheel.move(speed);
	m_Flywheel2.move(speed);
}

int getFlywheelRPM()
{
	return m_Flywheel.get_actual_velocity() * 15;
}

pros::Motor m_Intake(abs(e_Intake), e_Intake<0);
void setIntakePower(int speed)
{
	m_Intake.move(speed);
}

pros::Motor m_Indexer(abs(e_Indexer), e_Indexer<0);
void setIndexerPower(int speed)
{
	m_Indexer.move(speed);
}
