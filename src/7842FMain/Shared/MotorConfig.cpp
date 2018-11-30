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

pros::Controller j_Main(pros::E_CONTROLLER_MASTER);

//Flywheel -----------------------
okapi::Motor m_Flywheel(abs(em_Flywheel), em_Flywheel<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
okapi::Motor m_Flywheel2(abs(em_Flywheel2), em_Flywheel2<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
okapi::MotorGroup m_FlywheelGroup({m_Flywheel, m_Flywheel2});
void setFlywheelPower(int speed)
{
	m_FlywheelGroup.moveVoltage(speed/127*12000);
}

int getFlywheelRPM()
{
	return m_Flywheel.get_actual_velocity() * 15;
}

//Intake -----------------------
okapi::Motor m_Intake(abs(em_Intake), em_Intake<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
void setIntakePower(int speed)
{
	m_Intake.moveVoltage(speed/127*12000);
}

//Indexer -----------------------
okapi::Motor m_Indexer(abs(em_Indexer), em_Indexer<0, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
void setIndexerPower(int speed)
{
	m_Indexer.moveVoltage(speed/127*12000);
}


//Base -----------------------

std::shared_ptr<ChassisControllerPID> robotChassis = std::make_shared<ChassisControllerPID>
(
	TimeUtilFactory::create(),
	std::make_shared<ThreeEncoderSkidSteerModel>
	(
		std::make_shared<MotorGroup>(MotorGroup({em_LeftFront, em_LeftBack})),
		std::make_shared<MotorGroup>(MotorGroup({em_RightFront, em_RightBack})),
		std::make_shared<ADIEncoder>(ADIEncoder(es_BaseLeftEncoder, es_BaseLeftEncoder+1)),
		std::make_shared<ADIEncoder>(ADIEncoder(es_BaseBackEncoder, es_BaseBackEncoder+1)),
		std::make_shared<ADIEncoder>(ADIEncoder(es_BaseRightEncoder, es_BaseRightEncoder+1)),
		200, 12000
	),
	std::make_unique<IterativePosPIDController>(IterativeControllerFactory::posPID(0.0022, 0.00, 0)),
	std::make_unique<IterativePosPIDController>(IterativeControllerFactory::posPID(0.002, 0.0, 0)),
	std::make_unique<IterativePosPIDController>(IterativeControllerFactory::posPID(0.0016, 0, 0)),
	AbstractMotor::gearset::green,
	ChassisScales({4.125_in, 25.15_in})
);

void setBaseArcade(int yPower, int zPower)
{
	robotChassis->arcade(yPower/127, zPower/127, 0);
}

void setBasePower(int leftPower, int rightPower)
{
	robotChassis->tank(leftPower/127, rightPower/127, 0);
}
