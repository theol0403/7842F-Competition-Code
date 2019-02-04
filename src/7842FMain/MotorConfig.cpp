#include "MotorConfig.hpp"

#define motorEnum(x) abs(x),x<0 // Used to convert port number (negative for reversed) into motor constructor

okapi::Controller j_Main(okapi::ControllerId::master);

//Pointers to all devices
okapi::Motor* m_Flywheel = nullptr;
okapi::Motor* m_Flywheel2 = nullptr;

okapi::Motor* m_Intake = nullptr;
okapi::Motor* m_Indexer = nullptr;

pros::ADILineSensor* s_indexerSensor = nullptr;

okapi::ADIEncoder* s_leftEncoder = nullptr;
okapi::ADIEncoder* s_rightEncoder = nullptr;
okapi::ADIEncoder* s_middleEncoder = nullptr;

std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> model = nullptr;
std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile = nullptr;

lib7842::OdomTracker* tracker = nullptr;
lib7842::OdomController* chassis = nullptr;

lib7842::AutonSelector *autonSelector = nullptr;


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
	if(model == nullptr)
	{
		std::cout << "USING BASE BEFORE INIT\n";
		pros::delay(500);
	}
}

void setBaseArcade(double yPower, double zPower)
{
	checkBaseStatus();
	model->arcade(yPower, zPower, 0);
}

void setBasePower(double leftPower, double rightPower)
{
	checkBaseStatus();
	model->tank(leftPower, rightPower, 0);
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

	s_leftEncoder = new okapi::ADIEncoder(3, 4);
	s_rightEncoder = new okapi::ADIEncoder(5, 6);
	s_middleEncoder = new okapi::ADIEncoder(8, 7);

	s_leftEncoder->reset();
	s_rightEncoder->reset();
	s_middleEncoder->reset();

}


void initializeBase()
{

	model = std::make_shared<ThreeEncoderSkidSteerModel>(
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({e_m_LeftFront, e_m_LeftBack})),
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({e_m_RightFront, e_m_RightBack})),
		std::make_shared<ADIEncoder>(*s_leftEncoder),
		std::make_shared<ADIEncoder>(*s_middleEncoder),
		std::make_shared<ADIEncoder>(*s_rightEncoder),
		200,
		12000);


		tracker = new lib7842::OdomTracker
		(
			model,
			12.55_in, 8_in,
			2.75_in,
			360 * 1.6, 360
		);


		chassis = new lib7842::OdomController
		(
			tracker,
			new lib7842::PID(0.00001, 0, 1, 50, 5, 250_ms), //Distance PID - To mm
			new lib7842::PID(0.00005, 0, 1, 50, 5, 250_ms), //Angle PID - To Degree
			new lib7842::PID(0.00005, 0.005, 0.9, 3, 1, 100_ms) //Turn PID - To Degree
		);


		pros::delay(200);
		tracker->resetSensors();
		tracker->resetState();
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
		s_leftEncoder = new okapi::ADIEncoder(3, 4);
		s_rightEncoder = new okapi::ADIEncoder(5, 6);
		s_middleEncoder = new okapi::ADIEncoder(8, 7);

		s_leftEncoder->reset();
		s_rightEncoder->reset();
		s_middleEncoder->reset();
	}

	void initializeBase()
	{


		model = std::make_shared<ThreeEncoderSkidSteerModel>(
			std::make_shared<Motor>(left_mPort),
			std::make_shared<Motor>(right_mPort),
			std::make_shared<ADIEncoder>(*s_leftEncoder),
			std::make_shared<ADIEncoder>(*s_middleEncoder),
			std::make_shared<ADIEncoder>(*s_rightEncoder),
			200,
			12000);


			tracker = new lib7842::OdomTracker
			(
				model,
				27_cm, -18_cm,
				4_in,
				360, 360
			);


			chassis = new lib7842::OdomController
			(
				tracker,
				new lib7842::PID(0.00003, 0, 1, 40, 5, 250_ms), //Distance PID - To mm
				new lib7842::PID(0.00004, 0.00, 0, 3, 1, 100_ms), //Angle PID - To Degree
				new lib7842::PID(0.00005, 0.005, 0.9, 3, 1, 100_ms) //Turn PID - To Degree
			);


			pros::delay(200);
			tracker->resetSensors();
			tracker->resetState();
		}



		#endif //COMPETITION_ROBOT
