#include "MotorConfig.hpp"

const int8_t em_Flywheel = 11;
const int8_t em_Flywheel2 = -12;
const int8_t em_Intake = 1;
const int8_t em_Indexer = 2;

const int8_t em_RightFront = -5;
const int8_t em_RightBack = -6;
const int8_t em_LeftFront = 7;
const int8_t em_LeftBack = 8;

ADIEncoder leftEncoder(3, 4);
ADIEncoder rightEncoder(5, 6);
ADIEncoder middleEncoder(7, 8);



Controller j_Main(ControllerId::master);

//Flywheel -----------------------
Motor m_Flywheel(abs(em_Flywheel), em_Flywheel<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor m_Flywheel2(abs(em_Flywheel2), em_Flywheel2<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
MotorGroup m_FlywheelGroup({m_Flywheel, m_Flywheel2});

void setFlywheelPower(int speed)
{
	m_FlywheelGroup.moveVoltage(speed/127*12000);
}

int getFlywheelRPM()
{
	return m_Flywheel.get_actual_velocity() * 15;
}

//Intake -----------------------
Motor m_Intake(abs(em_Intake), em_Intake<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

void setIntakePower(int speed)
{
	m_Intake.moveVoltage(speed/127*12000);
}

//Indexer -----------------------
Motor m_Indexer(abs(em_Indexer), em_Indexer<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

void setIndexerPower(int speed)
{
	m_Indexer.moveVoltage(speed/127*12000);
}


//Base -----------------------
std::shared_ptr<okapi::OdomChassisController> robotChassis = ChassisControllerBuilder()
.withMotors(MotorGroup{em_LeftFront, em_LeftBack}, MotorGroup{em_RightFront, em_RightBack})
.withSensors(leftEncoder, rightEncoder)
.withMiddleEncoder(middleEncoder)
.withDimensions(ChassisScales{{2.75_in * 1.6, 12.9_in, 1_in, 2.75_in}, quadEncoderTPR})
.withOdometry()
.buildOdometry();


void setBaseArcade(float yPower, float zPower)
{
	robotChassis->arcade(yPower, zPower, 0);
}

void setBasePower(float leftPower, float rightPower)
{
	robotChassis->tank(leftPower, rightPower, 0);
}
