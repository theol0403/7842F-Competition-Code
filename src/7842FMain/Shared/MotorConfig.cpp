#include "MotorConfig.hpp"

const int8_t em_RightFront = -5;
const int8_t em_RightBack = -6;
const int8_t em_LeftFront = 7;
const int8_t em_LeftBack = 8;
const int8_t em_Flywheel = 11;
const int8_t em_Flywheel2 = -12;
const int8_t em_Intake = 1;
const int8_t em_Indexer = 2;

const int8_t es_BaseLeftEncoder = 1;
const int8_t es_BaseRightEncoder = 3;
const int8_t es_BaseBackEncoder = 5;

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

std::shared_ptr<OdomChassisController> robotChassis = ChassisControllerBuilder()
							 .withMotors({em_LeftFront, em_LeftBack}, {em_RightFront, em_RightBack})
							 .withGains(IterativePosPIDController::Gains{0.0022, 0.00, 0}, IterativePosPIDController::Gains{0.002, 0.0, 0}, IterativePosPIDController::Gains{0.0016, 0, 0})
							 .withSensors({es_BaseLeftEncoder, es_BaseLeftEncoder+1}, {es_BaseRightEncoder, es_BaseRightEncoder+1})
							 //.withMiddleEncoder({es_BaseBackEncoder, es_BaseBackEncoder+1})
							 .withDimensions({{2.75_in * 1.6, 12.9_in, //10_in
							 }, quadEncoderTPR})
							 .withOdometry()
							 .buildOdometry();


void setBaseArcade(float yPower, float zPower)
{
	robotChassis->arcade(yPower/127.0, zPower/127.0, 0);
}

void setBasePower(float leftPower, float rightPower)
{
	robotChassis->tank(leftPower/127.0, rightPower/127.0, 0);
}
