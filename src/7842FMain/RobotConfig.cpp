#include "RobotConfig.hpp"

okapi::Controller j_Main(okapi::ControllerId::master);
display_t display;
robot_t robot;

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
const int8_t mArm = 2;

const int8_t mRightFront = -19;
const int8_t mRightBack = -20;
const int8_t mLeftFront = 12;
const int8_t mLeftBack = 11;


void initializeBase()
{
	robot.printer = new ControllerPrinter(&j_Main);

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
		6.035_in, 2.75_in, 360,
		lib7842::OdomTracker::mdTracking
	);

	robot.chassis = new lib7842::OdomController (
		robot.tracker,
		new IterativePosPIDController(0.0014, 0, 0.000, 0, TimeUtilFactory::withSettledUtilParams(30, 5, 100_ms)), //Distance PID - To mm
		new IterativePosPIDController(0.007, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(50, 10, 100_ms)), //Angle PID - To Degree
		new IterativePosPIDController(0.0105, 0.0012, 0.0004, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)) //Turn PID - To Degree
	);

	pros::delay(200);
	robot.tracker->resetState();
	robot.tracker->resetSensors();
	robot.tracker->resetState();
}

const int globalFlywheelRPM = 2800;

void initializeDevices()
{

	robot.intake = new IntakeController(new okapi::Motor(mIntake), new okapi::Motor(mIndexer), new pros::ADILineSensor('D'), 1);

	Motor* flywheelM = new Motor(mFlywheel);
	robot.flywheel = new FlywheelController(
		robot.intake,
		flywheelM, new ADIEncoder('A', 'B', false),
		new VelMath(quadEncoderTPR / 3, std::make_shared<okapi::AverageFilter<4>>(), 10_ms, std::make_unique<Timer>()),
		new emaFilter(0.015),
		new lib7842::velPID(0.073, 0.35, 0.04, 0.1), 0.4
	);

	robot.shooter = new ShootController(robot.intake, robot.flywheel, new pros::ADIPotentiometer('C'), 32.3, new IterativePosPIDController(0.025, 0, 0, 0, TimeUtilFactory::create()));

	robot.arm = new ArmController(new okapi::Motor(mArm), new IterativePosPIDController(0.12, 0, 0, 0, TimeUtilFactory::create()));

	display.flywheel = new FlywheelTuner(display.main->newTab("Fly"));
	(*display.flywheel)
	.withButton("kP", &robot.flywheel->pid->m_Kp)
	.withButton("kD", &robot.flywheel->pid->m_Kd)
	.withButton("kF", &robot.flywheel->pid->m_Kf)
	.withButton("dEma", &robot.flywheel->pid->m_dFilter.m_alpha)
	.withButton("rEMA", &robot.flywheel->rpmFilter->m_alpha)
	.withButton("RPM", &robot.flywheel->targetRpm, FlywheelTuner::btnType::increment, 400)
	.withButton("Mult", &display.flywheel->multiplier, FlywheelTuner::btnType::multiply, 10)
	.withGauge("RPM", {&robot.flywheel->targetRpm, &robot.flywheel->currentRpm}, 0, 3000)
	.withGauge("Error", {&robot.flywheel->pid->m_Error}, -50, 50)
	.withGauge("Power", {&robot.flywheel->motorPower}, 0, 127)
	.build();

	display.graph = new Graph(display.main->newTab("Graph"), LV_COLOR_WHITE);
	(*display.graph)
	.withRange(-20, 20)
	.withRes(300)
	.withLines(10, 8)
	.withRefresh(20)
	// .withSeries("Target/6", &robot.flywheel->targetRpm, LV_COLOR_RED, 6)
	// .withSeries("Current/6", &robot.flywheel->currentRpm, LV_COLOR_BLUE, 6)
	// .withSeries("Power", &robot.flywheel->motorPower, LV_COLOR_GREEN)
	// .withSeries("D", &robot.flywheel->pid->m_derivative, LV_COLOR_PURPLE)
	.withSeries("Accel", &robot.flywheel->currentAccel, LV_COLOR_LIME)
	.build();

	//	robot.vision = new VisionController(new pros::Vision(4), display.main->newTab("Vision"));

	display.odom = new OdomDisplay(display.main->newTab("Odom"), robot.tracker);

	display.angler = new AngleTuner(display.main->newTab("Angler"), robot.shooter);

	//	display.chassisTuner = new ChassisTuner(display.main->newTab("Chassis"), robot.chassis);
	
	display.driverDisplay = new DriverDisplay(display.main->newTab("Driver"));

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

void initializeBase()
{

	robot.model = std::make_shared<SkidSteerModel> (
		std::make_shared<Motor>(left_mPort),
		std::make_shared<Motor>(right_mPort),
		std::make_shared<ADIEncoder>('C', 'D'),
		std::make_shared<ADIEncoder>('C', 'D'),
		200,
		12000
	);

	robot.tracker = new lib7842::OdomTracker (
		robot.model,
		5.88_in, 2.75_in, 360,
		lib7842::OdomTracker::mdTracking
	);

	robot.chassis = new lib7842::OdomController (
		robot.tracker,
		new IterativePosPIDController(0.003, 0, 0.000, 0, TimeUtilFactory::withSettledUtilParams(40, 5, 250_ms)), //Distance PID - To mm
		new IterativePosPIDController(0.005, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(50, 10, 100_ms)), //Angle PID - To Degree
		new IterativePosPIDController(0.008, 0, 0.000, 0, TimeUtilFactory::withSettledUtilParams(3, 1, 100_ms)) //Turn PID - To Degree
	);

	pros::delay(200);
	robot.tracker->resetState();
	robot.tracker->resetSensors();
	robot.tracker->resetState();
}

const int globalFlywheelRPM = 2800;

void initializeDevices()
{

	//	robot.vision = new VisionController(new pros::Vision(4), display.main->newTab("Vision"));

	display.odom = new OdomDisplay(display.main->newTab("Odom"), robot.tracker);

}

#endif //COMPETITION_ROBOT
