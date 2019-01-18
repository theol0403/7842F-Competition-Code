#include "MotorConfig.hpp"

#define motorEnum(x) abs(x),x<0 // Used to convert port number (negative for reversed) into motor constructor

okapi::Controller j_Main(okapi::ControllerId::master);

const int8_t e_m_Flywheel = 11;
const int8_t e_m_Flywheel2 = -12;
const int8_t e_m_Intake = -7;
const int8_t e_m_Indexer = 8;

const int8_t e_m_RightFront = -9;
const int8_t e_m_RightBack = -10;
const int8_t e_m_LeftFront = 19;
const int8_t e_m_LeftBack = 20;


okapi::Motor* m_Flywheel = nullptr;
okapi::Motor* m_Flywheel2 = nullptr;

okapi::Motor* m_Intake = nullptr;
okapi::Motor* m_Indexer = nullptr;

pros::ADILineSensor* s_indexerSensor = nullptr;

okapi::ADIEncoder* s_leftEncoder = nullptr;
okapi::ADIEncoder* s_rightEncoder = nullptr;
okapi::ADIEncoder* s_middleEncoder = nullptr;

lib7842::AutonSelector *autonSelector = nullptr;

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

	autonSelector = new lib7842::AutonSelector(lv_scr_act(), {{"None", AutoNothing}, {"Close", AutoClose}, {"Middle", AutoMiddle}, {"Far", AutoFar}});

}


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


//Base -----------------------
std::shared_ptr<okapi::ChassisControllerPID> robotChassis = nullptr;
std::shared_ptr<okapi::AsyncMotionProfileController> robotProfile = nullptr;

lib7842::OdomTracker* chassisOdomTracker = nullptr;
lib7842::OdomController* chassisOdomController = nullptr;

void initializeBase()
{
	const QLength chassisWidth = 12.55_in;

	// Otherwise, you should specify the gearset and scales for your robot
	// robotChassis = ChassisControllerFactory::createPtr(
	// 	{e_m_LeftFront, e_m_LeftBack}, {e_m_RightFront, e_m_RightBack},
	// 	*s_leftEncoder, *s_rightEncoder,
	// 	IterativePosPIDController::Gains{0.001, 0.00, 0},
	// 	IterativePosPIDController::Gains{0.0006, 0.00, 0},
	// 	IterativePosPIDController::Gains{0.0006, 0, 0},
	// 	AbstractMotor::gearset::green,
	// 	{2.75_in / 1.6, chassisWidth * 2}
	// );

	std::shared_ptr<SkidSteerModel> robotModel = std::make_shared<SkidSteerModel>(
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({e_m_LeftFront, e_m_LeftBack})),
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({e_m_RightFront, e_m_RightBack})),
		std::make_shared<ADIEncoder>(*s_leftEncoder), std::make_shared<ADIEncoder>(*s_rightEncoder),
		200,
		12000
	);

	robotChassis = std::make_shared<ChassisControllerPID>(
		TimeUtilFactory::create(),
		robotModel,
		std::make_unique<IterativePosPIDController>(0.001, 0.00, 0, 0, TimeUtilFactory::withSettledUtilParams(50, 5, 100_ms)),
		std::make_unique<IterativePosPIDController>(0.0006, 0.00, 0, 0, TimeUtilFactory::withSettledUtilParams(50, 5, 100_ms)),
		std::make_unique<IterativePosPIDController>(0.0006, 0.00, 0, 0, TimeUtilFactory::withSettledUtilParams(50, 5, 100_ms)),
		AbstractMotor::gearset::green,
		ChassisScales({2.75_in / 1.6, chassisWidth * 2}));

		//	robotChassis->setMaxVelocity(180);

		chassisOdomTracker = new lib7842::OdomTracker
		(
			s_leftEncoder, s_rightEncoder, s_middleEncoder,
			chassisWidth, 8_in,
			2.75_in,
			360 * 1.6, 360
		);

		chassisOdomController = new lib7842::OdomController
		(
			robotChassis,
			chassisOdomTracker
		);


		// robotProfile = std::make_shared<AsyncMotionProfileController>(AsyncControllerFactory::motionProfile
		// 	(
		// 		1.0, 2.0, 10.0,
		// 		*robotChassis
		// 	));

		pros::delay(500);

		chassisOdomTracker->resetSensors();
		chassisOdomTracker->resetState();
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
