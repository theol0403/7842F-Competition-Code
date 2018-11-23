#include "MotorConfig.hpp"

const int8_t e_RightFront = -5;
const int8_t e_RightBack = -6;
const int8_t e_LeftFront = 7;
const int8_t e_LeftBack = 8;
const int8_t e_Flywheel = 11;
const int8_t e_Flywheel2 = -12;
const int8_t e_Intake = 1;
const int8_t e_Indexer = 2;

pros::Controller j_Main(pros::E_CONTROLLER_MASTER);

pros::Motor m_RightFront(abs(e_RightFront), e_RightFront<0);
pros::Motor m_RightBack(abs(e_RightBack), e_RightBack<0);
pros::Motor m_LeftFront(abs(e_LeftFront), e_LeftFront<0);
pros::Motor m_LeftBack(abs(e_LeftBack), e_LeftBack<0);

void setBasePower(int yPower, int xPower, int zPower)
{
	m_RightFront.move(yPower+xPower-zPower);
	m_RightBack.move(yPower-xPower-zPower);
	m_LeftFront.move(yPower+xPower+zPower);
	m_LeftBack.move(yPower-xPower+zPower);
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
