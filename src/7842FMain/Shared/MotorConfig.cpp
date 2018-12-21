#include "MotorConfig.hpp"

#define motorEnum(x) abs(x),x<0 // Used to convert port number (negative for reversed) into motor constructor

okapi::Controller j_Main(okapi::ControllerId::master);

const int8_t e_m_Flywheel = 1;
const int8_t e_m_Flywheel2 = -16;
const int8_t e_m_Intake = -4;
const int8_t e_m_Indexer = -19;

const int8_t e_m_RightFront = -7;
const int8_t e_m_RightBack = -8;
const int8_t e_m_LeftFront = 17;
const int8_t e_m_LeftBack = 18;


okapi::Motor* m_Flywheel = nullptr;
okapi::Motor* m_Flywheel2 = nullptr;
okapi::MotorGroup* m_FlywheelGroup = nullptr;

okapi::Motor* m_Intake = nullptr;
okapi::Motor* m_Indexer = nullptr;

pros::ADILineSensor* s_indexerSensor = nullptr;

okapi::ADIEncoder leftEncoder(3, 4);
okapi::ADIEncoder rightEncoder(5, 6);
okapi::ADIEncoder middleEncoder(7, 8);


void initializeDevices()
{
	m_Flywheel = new okapi::Motor(motorEnum(e_m_Flywheel), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_Flywheel2 = new okapi::Motor(motorEnum(e_m_Flywheel2), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_FlywheelGroup = new okapi::MotorGroup({*m_Flywheel, *m_Flywheel2});

	m_Intake = new okapi::Motor(motorEnum(e_m_Intake), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_Indexer = new okapi::Motor(motorEnum(e_m_Indexer), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_Indexer->setBrakeMode(AbstractMotor::brakeMode::hold);

	s_indexerSensor = new pros::ADILineSensor('A');
	s_indexerSensor->calibrate();

}


void setFlywheelPower(double speed) {	m_FlywheelGroup->moveVoltage(speed/127.0*12000); }
double getFlywheelRPM() { return m_FlywheelGroup->getActualVelocity() * 15; } // 1:15 ratio from motor output to flywhel speed

void setIntakePower(double speed) { m_Intake->moveVoltage(speed/127.0*12000); }
void setIndexerPower(double speed) { m_Indexer->moveVoltage(speed/127.0*12000); }
void setIndexerVelocity(double speed) {	m_Indexer->moveVelocity(speed); }

double getIndexerSensor() {	return s_indexerSensor->get_value_calibrated(); }


//Base -----------------------
std::shared_ptr<okapi::OdomChassisController> robotChassis = nullptr;
std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile = nullptr;

void initializeBase()
{
	robotChassis = okapi::ChassisControllerBuilder()
	.withMotors({e_m_LeftFront, e_m_LeftBack}, {e_m_RightFront, e_m_RightBack})
	.withSensors(leftEncoder, rightEncoder)
	.withMiddleEncoder(middleEncoder)
	.withDimensions(ChassisScales{{2.75_in, 12.9_in, 1_in, 2.75_in}, okapi::quadEncoderTPR})
//	.withGains({0.0022, 0.00, 0}, {0.002, 0.0, 0}, {0.0016, 0, 0})
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
