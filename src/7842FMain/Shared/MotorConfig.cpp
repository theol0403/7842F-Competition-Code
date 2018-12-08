#include "MotorConfig.hpp"

#define motorEnum(x) abs(x),x<0 // Used to convert port number (negative for reversed) doubleo motor constructor

Controller j_Main(ControllerId::master);

const int8_t e_m_Flywheel = 11;
const int8_t e_m_Flywheel2 = -12;
const int8_t e_m_Intake = 1;
const int8_t e_m_Indexer = 2;

const int8_t e_m_RightFront = -5;
const int8_t e_m_RightBack = -6;
const int8_t e_m_LeftFront = 7;
const int8_t e_m_LeftBack = 8;

ADIEncoder leftEncoder(3, 4);
ADIEncoder rightEncoder(5, 6);
ADIEncoder middleEncoder(7, 8);



//Flywheel -----------------------
Motor m_Flywheel(motorEnum(e_m_Flywheel), AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor m_Flywheel2(motorEnum(e_m_Flywheel2), AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
MotorGroup m_FlywheelGroup({m_Flywheel, m_Flywheel2});

void setFlywheelPower(double speed)
{
	m_FlywheelGroup.moveVoltage(speed/127.0*12000);
}

double getFlywheelRPM()
{
	return m_FlywheelGroup.getActualVelocity() * 15; // 1:15 ratio from motor output to flywhel speed
}

//Intake -----------------------
Motor m_Intake(motorEnum(e_m_Intake), AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

void setIntakePower(double speed)
{
	m_Intake.moveVoltage(speed/127.0*12000);
}

//Indexer -----------------------
Motor m_Indexer(motorEnum(e_m_Indexer), AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

void setIndexerPower(double speed)
{
	m_Indexer.moveVoltage(speed/127.0*12000);
}


//Base -----------------------
std::shared_ptr<okapi::OdomChassisController> robotChassis = nullptr;
std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile = nullptr;

void initializeBase()
{
	robotChassis = ChassisControllerBuilder()
	.withMotors(MotorGroup{e_m_LeftFront, e_m_LeftBack}, MotorGroup{e_m_RightFront, e_m_RightBack})
	.withSensors(leftEncoder, rightEncoder)
	.withMiddleEncoder(middleEncoder)
	.withDimensions(ChassisScales{{2.75_in * 1.6, 12.9_in, 1_in, 2.75_in}, quadEncoderTPR})
	.withOdometry()
	.buildOdometry();

	robotProfile = AsyncMotionProfileControllerBuilder()
	.withOutput(robotChassis)
	.withLimits(PathfinderLimits{1.0, 2.0, 10.0})
	.buildMotionProfileController();

	robotChassis->setState(OdomState{0_ft, 0_ft, 0_deg});
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
