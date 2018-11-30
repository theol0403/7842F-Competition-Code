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


std::shared_ptr<ChassisControllerPID> robotChassis = ChassisControllerFactory::createPtr(
	{e_LeftFront, e_LeftBack}, {e_RightFront, e_RightBack},
	IterativePosPIDController::Gains{0.0022, 0.00, 0}, //Driving PID
	IterativePosPIDController::Gains{0.002, 0.0, 0}, //Angle PID
	IterativePosPIDController::Gains{0.0016, 0, 0}, //Turning PID
	AbstractMotor::gearset::green, {4_in, 25.15_in} //Wheel Diam, Chassis Width
);

void setBasePower(int yPower, int zPower)
{
	robotChassis->arcade(yPower/127, zPower/127, 0);
}


okapi::Motor m_Flywheel(abs(e_Flywheel), e_Flywheel<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
okapi::Motor m_Flywheel2(abs(e_Flywheel2), e_Flywheel2<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
okapi::MotorGroup m_FlywheelGroup({m_Flywheel, m_Flywheel2});

void setFlywheelPower(int speed)
{
	m_FlywheelGroup.moveVoltage(speed/127*12000);
}

int getFlywheelRPM()
{
	return m_Flywheel.get_actual_velocity() * 15;
}

okapi::Motor m_Intake(abs(e_Intake), e_Intake<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
void setIntakePower(int speed)
{
	m_Intake.moveVoltage(speed/127*12000);
}

okapi::Motor m_Indexer(abs(e_Indexer), e_Indexer<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
void setIndexerPower(int speed)
{
	m_Indexer.moveVoltage(speed/127*12000);
}
