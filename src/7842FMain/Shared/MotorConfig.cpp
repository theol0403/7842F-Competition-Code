#include "MotorConfig.hpp"

#define motorEnum(x) abs(x),x<0 // Used to convert port number (negative for reversed) into motor constructor

okapi::Controller j_Main(okapi::ControllerId::master);

const int8_t e_m_Flywheel = 11;
const int8_t e_m_Flywheel2 = -12;
const int8_t e_m_Intake = 1;
const int8_t e_m_Indexer = 2;

const int8_t e_m_RightFront = -5;
const int8_t e_m_RightBack = -6;
const int8_t e_m_LeftFront = 7;
const int8_t e_m_LeftBack = 8;

okapi::ADIEncoder leftEncoder(3, 4);
okapi::ADIEncoder rightEncoder(5, 6);
okapi::ADIEncoder middleEncoder(7, 8);


//Flywheel -----------------------
okapi::Motor m_Flywheel(motorEnum(e_m_Flywheel), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::Motor m_Flywheel2(motorEnum(e_m_Flywheel2), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
okapi::MotorGroup m_FlywheelGroup({m_Flywheel, m_Flywheel2});

void setFlywheelPower(double speed)
{
	m_FlywheelGroup.moveVoltage(speed/127.0*12000);
}

double getFlywheelRPM()
{
	return m_FlywheelGroup.getActualVelocity() * 15; // 1:15 ratio from motor output to flywhel speed
}

//Intake -----------------------
okapi::Motor m_Intake(motorEnum(e_m_Intake), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);

void setIntakePower(double speed)
{
	m_Intake.moveVoltage(speed/127.0*12000);
}

//Indexer -----------------------
okapi::Motor m_Indexer(motorEnum(e_m_Indexer), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);

void setIndexerPower(double speed)
{
	m_Indexer.moveVoltage(speed/127.0*12000);
}


//Base -----------------------
std::shared_ptr<okapi::OdomChassisController> robotChassis = nullptr;
std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile = nullptr;

void initializeBase()
{
	robotChassis = okapi::ChassisControllerBuilder()
	.withMotors({e_m_LeftFront, e_m_LeftBack}, {e_m_RightFront, e_m_RightBack})
	.withSensors(leftEncoder, rightEncoder)
	.withMiddleEncoder(middleEncoder)
	.withDimensions({{2.75_in * 1.6, 12.9_in, 1_in, 2.75_in}, okapi::quadEncoderTPR})
	.withGains({0.0022, 0.00, 0}, {0.002, 0.0, 0}, {0.0016, 0, 0})
	.withOdometry()
	.buildOdometry();

	robotProfile = okapi::AsyncMotionProfileControllerBuilder()
	.withOutput(robotChassis)
	.withLimits({1.0, 2.0, 10.0})
	.buildMotionProfileController();

	robotChassis->setState(okapi::OdomState{0_ft, 0_ft, 0_deg});
}

void checkBaseStatus()
{
	if(robotChassis == nullptr)
	{
		std::cout << "USING BASE BEFORE INIT\n";
		pros::delay(500);
	}
}


void setBaseArcade(double yPower, double zPower)
{
	checkBaseStatus();
	robotChassis->arcade(yPower, zPower, 0);
}

void setBasePower(double leftPower, double rightPower)
{
	checkBaseStatus();
	robotChassis->tank(leftPower, rightPower, 0);
}
