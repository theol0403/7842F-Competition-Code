#include "MotorConfig.hpp"

okapi::Controller j_Main(okapi::ControllerId::master);

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
const int8_t mFlywheel = -1;
const int8_t mIntake = -9;
const int8_t mIndexer = 10;
const int8_t mArm = -8;

const int8_t mRightFront = -19;
const int8_t mRightBack = -20;
const int8_t mLeftFront = 12;
const int8_t mLeftBack = 11;


void initializeBase()
{

	robot.model = std::make_shared<SkidSteerModel> (
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({mLeftFront, mLeftBack})),
		std::make_shared<MotorGroup>(std::initializer_list<Motor>({mRightFront, mRightBack})),
		std::make_shared<ADIEncoder>('H', 'G'),
		std::make_shared<ADIEncoder>('F', 'E'),
		200,
		12000
	);

	robot.tracker = new lib7842::OdomTracker (
		robot.model,
		6.4375_in, 2.75_in, 360,
		lib7842::OdomTracker::mdTracking
	);

	robot.chassis = new lib7842::OdomController (
		robot.tracker,
		new IterativePosPIDController(0.0016, 0, 0.000, 0, TimeUtilFactory::withSettledUtilParams(40, 5, 250_ms), std::make_unique<AverageFilter<5>>()), //Distance PID - To mm
		new IterativePosPIDController(0.005, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(10, 5, 100_ms), std::make_unique<AverageFilter<5>>()), //Angle PID - To Degree
		new IterativePosPIDController(0.003, 0, 0.000, 0, TimeUtilFactory::withSettledUtilParams(0.1, 0, 250_ms), std::make_unique<AverageFilter<5>>()) //Turn PID - To Degree
	);

	// new lib7842::PID(0.002, 0.01, 1, 40, 5, 250_ms), //Distance PID - To mm
	// new lib7842::PID(0.004, 0.00, 1, 3, 1, 100_ms), //Angle PID - To Degree
	// new lib7842::PID(0.008, 0.01, 1, 0.1, 0, 250_ms) //Turn PID - To Degree

}

const int globalFlywheelRPM = 2900;

void initializeDevices()
{
	robot.intake = new IntakeController(new okapi::Motor(mIntake), new okapi::Motor(mIndexer), new pros::ADILineSensor('D'), 1);

	robot.flywheel = new FlywheelController(robot.intake, new okapi::Motor(mFlywheel), 15, new lib7842::velPID(0.35, 0.05, 0.045, 0.9), new lib7842::emaFilter(0.15), 0.7);

	robot.shooter = new ShootController(robot.intake, robot.flywheel, new pros::ADIPotentiometer('C'), 138.1);

	//robot.arm = new ArmController(new okapi::Motor(mArm), new pros::ADIPotentiometer('A'), 26, new IterativePosPIDController(0.02, 0, 0, 0, TimeUtilFactory::create()));
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

	robot.model = std::make_shared<SkidSteerModel> (
		std::make_shared<Motor>(left_mPort),
		std::make_shared<Motor>(right_mPort),
		std::make_shared<ADIEncoder>(3, 4),
		std::make_shared<ADIEncoder>(5, 6),
		200,
		12000
	);

	robot.tracker = new lib7842::OdomTracker (
		robot.model,
		27_cm, 4_in, 360,
		lib7842::OdomTracker::mdTracking
	);

	robot.chassis = new lib7842::OdomController (
		robot.tracker,
		new IterativePosPIDController(0.009, 0, 0.0004, 0, TimeUtilFactory::withSettledUtilParams(40, 5, 250_ms), std::make_unique<AverageFilter<5>>()), //Distance PID - To mm
		new IterativePosPIDController(0.008, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(3, 1, 100_ms)), //Angle PID - To Degree
		new IterativePosPIDController(0.01, 0, 0.0004, 0, TimeUtilFactory::withSettledUtilParams(3, 1, 100_ms)) //Turn PID - To Degree
	);

}

#endif //COMPETITION_ROBOT
