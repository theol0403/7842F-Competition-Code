#include "MotorConfig.hpp"

#define motorEnum(x) abs(x),x<0 // Used to convert port number (negative for reversed) into motor constructor

okapi::Controller j_Main(okapi::ControllerId::master);

//Pointers to all devices
okapi::Motor* m_Flywheel = nullptr;
okapi::Motor* m_Flywheel2 = nullptr;

okapi::Motor* m_Intake = nullptr;
okapi::Motor* m_Indexer = nullptr;

pros::ADILineSensor* s_indexerSensor = nullptr;

robot_t robot;

void checkBaseStatus()
{
	if(robot.model == nullptr)
	{
		std::cout << "USING BASE BEFORE INIT\n";
		pros::delay(500);
	}
}

/***
*     _____                            _   _ _   _              ______      _           _
*    /  __ \                          | | (_) | (_)             | ___ \    | |         | |
*    | /  \/ ___  _ __ ___  _ __   ___| |_ _| |_ _  ___  _ __   | |_/ /___ | |__   ___ | |_
*    | |    / _ \| '_ ` _ \| '_ \ / _ \ __| | __| |/ _ \| '_ \  |    // _ \| '_ \ / _ \| __|
*    | \__/\ (_) | | | | | | |_) |  __/ |_| | |_| | (_) | | | | | |\ \ (_) | |_) | (_) | |_
*     \____/\___/|_| |_| |_| .__/ \___|\__|_|\__|_|\___/|_| |_| \_| \_\___/|_.__/ \___/ \__|
*                          | |
*                          |_|
*/
#ifndef TEST_ROBOT //Competition
const int8_t e_m_Flywheel = 11;
const int8_t e_m_Flywheel2 = -12;
const int8_t e_m_Intake = -7;
const int8_t e_m_Indexer = 8;

const int8_t e_m_RightFront = -9;
const int8_t e_m_RightBack = -10;
const int8_t e_m_LeftFront = 19;
const int8_t e_m_LeftBack = 20;


void initializeDevices()
{
	m_Flywheel = new okapi::Motor(motorEnum(e_m_Flywheel), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_Flywheel2 = new okapi::Motor(motorEnum(e_m_Flywheel2), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);

	m_Intake = new okapi::Motor(motorEnum(e_m_Intake), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_Indexer = new okapi::Motor(motorEnum(e_m_Indexer), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_Intake->setBrakeMode(AbstractMotor::brakeMode::hold);
	m_Indexer->setBrakeMode(AbstractMotor::brakeMode::hold);

	s_indexerSensor = new pros::ADILineSensor('A');
	s_indexerSensor->calibrate();
}


void initializeBase()
{

	robot.model = std::make_shared<ThreeEncoderSkidSteerModel>(
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({e_m_LeftFront, e_m_LeftBack})),
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({e_m_RightFront, e_m_RightBack})),
		std::make_shared<ADIEncoder>(3, 4),
		std::make_shared<ADIEncoder>(8, 7),
		std::make_shared<ADIEncoder>(5, 6),
		200,
		12000);


		robot.tracker = new lib7842::OdomTracker
		(
			robot.model,
			12.55_in, 8_in,
			2.75_in,
			360 * 1.6, 360
		);


		robot.chassis = new lib7842::OdomController
		(
			robot.tracker,
			new lib7842::PID(0.00001, 0, 1, 50, 5, 250_ms), //Distance PID - To mm
			new lib7842::PID(0.00005, 0, 1, 50, 5, 250_ms), //Angle PID - To Degree
			new lib7842::PID(0.00005, 0.005, 0.9, 3, 1, 100_ms) //Turn PID - To Degree
		);

	}



	/***
	*     _____         _    ______      _           _
	*    |_   _|       | |   | ___ \    | |         | |
	*      | | ___  ___| |_  | |_/ /___ | |__   ___ | |_
	*      | |/ _ \/ __| __| |    // _ \| '_ \ / _ \| __|
	*      | |  __/\__ \ |_  | |\ \ (_) | |_) | (_) | |_
	*      \_/\___||___/\__| \_| \_\___/|_.__/ \___/ \__|
	*
	*
	*/
	#else //TEST_ROBOT

	const int8_t left_mPort = 1;
	const int8_t right_mPort = -2;


	void initializeDevices()
	{
	}

	void initializeBase()
	{

		robot.model = std::make_shared<ThreeEncoderSkidSteerModel>(
			std::make_shared<Motor>(left_mPort),
			std::make_shared<Motor>(right_mPort),
			std::make_shared<ADIEncoder>(3, 4),
			std::make_shared<ADIEncoder>(1, 2),
			std::make_shared<ADIEncoder>(5, 6),
			200,
			12000);


			robot.tracker = new lib7842::OdomTracker
			(
				robot.model,
				27_cm, 0_cm,
				4_in,
				360, 360
			);


			robot.chassis = new lib7842::OdomController
			(
				robot.tracker,
				new lib7842::PID(0.008, 0, 1, 40, 5, 250_ms), //Distance PID - To mm
				new lib7842::PID(0.008, 0.00, 0, 3, 1, 100_ms), //Angle PID - To Degree
				new lib7842::PID(0.01, 0.005, 0.9, 3, 1, 100_ms) //Turn PID - To Degree
			);

		}

		#endif //COMPETITION_ROBOT
