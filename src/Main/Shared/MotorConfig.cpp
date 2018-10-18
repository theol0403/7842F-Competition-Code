#include "main.h"
#include "Include/Shared/MotorConfig.hpp"




pros::Controller j_Main(pros::E_CONTROLLER_MASTER);


pros::Motor m_RightBase(2, true);
pros::Motor m_RightBase2(3, true);

pros::Motor m_LeftBase(4);
pros::Motor m_LeftBase2(5);

pros::Motor m_hBase(6);





void setBasePower(int leftSpeed, int rightSpeed)
{
	m_RightBase.move(rightSpeed);
	m_RightBase2.move(rightSpeed);
	m_LeftBase.move(leftSpeed);
	m_LeftBase2.move(leftSpeed);
}

void setHPower(int hSpeed)
{
	m_hBase.move(hSpeed);
}
