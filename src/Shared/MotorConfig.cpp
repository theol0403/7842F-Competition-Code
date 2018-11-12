#include "main.h"
#include "Include/Shared/MotorConfig.hpp"




pros::Controller j_Main(pros::E_CONTROLLER_MASTER);


pros::Motor m_RightBase(2, true);
pros::Motor m_RightBase2(3, true);

pros::Motor m_LeftBase(4);
pros::Motor m_LeftBase2(5);

pros::Motor m_hBase(6, true);

pros::Motor m_Flywheel(7);

pros::Motor m_Intake(8);






void setBasePower(int leftPower, int rightPower)
{
	m_RightBase.move(rightPower);
	m_RightBase2.move(rightPower);
	m_LeftBase.move(leftPower);
	m_LeftBase2.move(leftPower);
}

void setHPower(int speed)
{
	m_hBase.move(speed);
}

void setFlywheelPower(int speed)
{
	m_Flywheel.move(speed);
}

int getFlywheelRPM()
{
	return m_Flywheel.get_actual_velocity() * 15;
}


void setIntakePower(int speed)
{
	m_Intake.move(speed);
}
