#include "MotorConfig.hpp"

#define motorEnum(x) abs(x),x<0 // Used to convert port number (negative for reversed) into motor constructor

okapi::Controller j_Main(okapi::ControllerId::master);

std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> robotChassis = nullptr;
std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile = nullptr;

lib7842::OdomTracker* chassisOdomTracker = nullptr;
lib7842::OdomController* chassisOdomController = nullptr;

okapi::Motor* m_Flywheel = nullptr;
okapi::Motor* m_Flywheel2 = nullptr;

okapi::Motor* m_Intake = nullptr;
okapi::Motor* m_Indexer = nullptr;

pros::ADILineSensor* s_indexerSensor = nullptr;

okapi::ADIEncoder* s_leftEncoder = nullptr;
okapi::ADIEncoder* s_rightEncoder = nullptr;
okapi::ADIEncoder* s_middleEncoder = nullptr;


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
#ifndef TEST_ROBOT
const int8_t Flywheel_mPort = 1;
const int8_t Flywheel2_mPort = -16;
const int8_t Intake_mPort = -4;
const int8_t Indexer_mPort = 19;

const int8_t leftFront_mPort = 17;
const int8_t leftBack_mPort = 18;
const int8_t rightFront_mPort = -7;
const int8_t rightBack_mPort = -8;


void initializeDevices()
{
	m_Flywheel = new okapi::Motor(motorEnum(Flywheel_mPort), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_Flywheel2 = new okapi::Motor(motorEnum(Flywheel2_mPort), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);


	m_Intake = new okapi::Motor(motorEnum(Intake_mPort), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_Indexer = new okapi::Motor(motorEnum(Indexer_mPort), okapi::AbstractMotor::gearset::green, okapi::AbstractMotor::encoderUnits::degrees);
	m_Intake->setBrakeMode(AbstractMotor::brakeMode::hold);
	m_Indexer->setBrakeMode(AbstractMotor::brakeMode::hold);

	s_indexerSensor = new pros::ADILineSensor('A');
	s_indexerSensor->calibrate();

	s_leftEncoder = new okapi::ADIEncoder(3, 4);
	s_rightEncoder = new okapi::ADIEncoder(5, 6);
	s_middleEncoder = new okapi::ADIEncoder(8, 7);

	s_leftEncoder->reset();
	s_rightEncoder->reset();
	s_middleEncoder->reset();

}


void initializeBase()
{

	robotChassis = std::make_shared<ThreeEncoderSkidSteerModel>(
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({leftFront_mPort, leftBack_mPort})),
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({rightFront_mPort, rightBack_mPort})),
		std::make_shared<ADIEncoder>(*s_leftEncoder),
		std::make_shared<ADIEncoder>(*s_middleEncoder),
		std::make_shared<ADIEncoder>(*s_rightEncoder),
		200,
		12000);


		chassisOdomTracker = new lib7842::OdomTracker
		(
			robotChassis,
			12.55_in, 8_in,
			2.75_in,
			360 * 1.6, 360
		);

		chassisOdomController = new lib7842::OdomController
		(
			chassisOdomTracker,
			new lib7842::PID(0.01, 0.1, 1, 50, 5, 250_ms), //Distance PID - To mm
			new lib7842::PID(0.01, 0.1, 1, 50, 5, 250_ms), //Angle PID - To Degree
			new lib7842::PID(0.0001, 1, 1, 5, 1, 250_ms) //Turn PID - To Degree
		);

		pros::delay(500);

		chassisOdomTracker->resetSensors();
		chassisOdomTracker->resetState();
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
	#else //COMPETITION_ROBOT


	const int8_t left_mPort = 1;
	const int8_t right_mPort = -2;


	void initializeDevices()
	{
		s_leftEncoder = new okapi::ADIEncoder(3, 4);
		s_rightEncoder = new okapi::ADIEncoder(5, 6);
		s_middleEncoder = new okapi::ADIEncoder(8, 7);

		s_leftEncoder->reset();
		s_rightEncoder->reset();
		s_middleEncoder->reset();
	}

	void initializeBase()
	{
		robotChassis = std::make_shared<ThreeEncoderSkidSteerModel>(
			std::make_shared<MotorGroup>(std::initializer_list<Motor>({left_mPort})),
			std::make_shared<MotorGroup>(std::initializer_list<Motor>({right_mPort})),
			std::make_shared<ADIEncoder>(*s_leftEncoder),
			std::make_shared<ADIEncoder>(*s_middleEncoder),
			std::make_shared<ADIEncoder>(*s_rightEncoder),
			200,
			12000);

			chassisOdomTracker = new lib7842::OdomTracker (
				robotChassis,
				27_cm, 18_cm,
				4_in,
				360, 360
			);

			chassisOdomController = new lib7842::OdomController (
				chassisOdomTracker,
				new lib7842::PID(0.01, 0.1, 1, 50, 5, 250_ms), //Distance PID - To mm
				new lib7842::PID(0.01, 0.1, 1, 50, 5, 250_ms), //Angle PID - To Degree
				new lib7842::PID(0.0001, 1, 1, 5, 1, 250_ms) //Turn PID - To Degree
			);

			pros::delay(500);
			chassisOdomTracker->resetSensors();
			chassisOdomTracker->resetState();
		}


		#endif //COMPETITION_ROBOT





		void setFlywheelPower(double speed)
		{
			m_Flywheel->moveVoltage(speed/127.0*12000);
			m_Flywheel2->moveVoltage(speed/127.0*12000);
		}
		double getFlywheelRPM() { return m_Flywheel->getActualVelocity() * 15; } // 1:15 ratio from motor output to flywhel speed

		void setIntakePower(double speed) { m_Intake->moveVoltage(speed/127.0*12000); }
		void setIndexerPower(double speed) { m_Indexer->moveVoltage(speed/127.0*12000); }
		void setIntakeVelocity(double speed) { m_Intake->moveVelocity(speed); }
		void setIndexerVelocity(double speed) {	m_Indexer->moveVelocity(speed); }

		double getIndexerSensor() {	return s_indexerSensor->get_value_calibrated(); }


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
