#include "RobotConfig.hpp"

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
#ifdef F_ROBOT //Competition
const int8_t mFlywheel = -1;
const int8_t mIntake = -9;
const int8_t mIndexer = 10;
const int8_t mArm = 2;

const int8_t mRightFront = -19;
const int8_t mRightBack = -20;
const int8_t mLeftFront = 12;
const int8_t mLeftBack = 11;
#endif

#ifdef B_ROBOT
const int8_t mFlywheel = -8;
const int8_t mIntake = 7;
const int8_t mIndexer = 9;
const int8_t mArm = 2;

const int8_t mRightFront = -20;
const int8_t mRightBack = -19;
const int8_t mLeftFront = 18;
const int8_t mLeftBack = 17;
#endif


const int globalFlywheelRPM = 3000;


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
		6.035_in, 2.75_in, 360,
		lib7842::OdomTracker::mdTracking
	);

	#ifdef F_ROBOT //Competition
	robot.chassis = new lib7842::OdomController (
		robot.tracker,
		new IterativePosPIDController(0.0022, 0.0002, 0.00013, 0, TimeUtilFactory::withSettledUtilParams(20, 5, 50_ms)), //Distance PID - To mm
		new IterativePosPIDController(0.007, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(50, 10, 100_ms)), //Angle PID - To Degree
		new IterativePosPIDController(0.0105, 0.0012, 0.0004, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)), //Turn PID - To Degree
		0.021
	);
	#endif

	#ifdef B_ROBOT
	robot.chassis = new lib7842::OdomController (
		robot.tracker,
		new IterativePosPIDController(0.0025, 0.00022, 0.00013, 0, TimeUtilFactory::withSettledUtilParams(20, 5, 50_ms)), //Distance PID - To mm
		new IterativePosPIDController(0.007, 0, 0, 0, TimeUtilFactory::withSettledUtilParams(50, 10, 100_ms)), //Angle PID - To Degree
		new IterativePosPIDController(0.007, 0.00, 0.0004, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms)), //Turn PID - To Degree
		0.021
	);
	#endif


	pros::delay(200);
	robot.tracker->resetState();
	robot.tracker->resetSensors();
	robot.tracker->resetState();
}


void initializeDevices()
{
	robot.printer = new ControllerPrinter();

	robot.intake = new IntakeController(new okapi::Motor(mIntake), new okapi::Motor(mIndexer), new pros::ADILineSensor('D'), 1);

	robot.flywheel = new FlywheelController(
		robot.intake,
		new Motor(mFlywheel), new ADIEncoder('A', 'B', false),
		new VelMath(quadEncoderTPR / 3, std::make_shared<okapi::AverageFilter<4>>(), 10_ms, std::make_unique<Timer>()),
		new emaFilter(0.015),
		new lib7842::velPID(0.073, 0.35, 0.04, 0.1), 0.6
	);

	#ifdef F_ROBOT
	robot.shooter = new ShootController(robot.intake, robot.flywheel, new pros::ADIPotentiometer('C'), 75.3 - 61, new IterativePosPIDController(0.015, 0, 0, 0, TimeUtilFactory::create()));
	#else
	robot.shooter = new ShootController(robot.intake, robot.flywheel, new pros::ADIPotentiometer('C'), 75.3 - 6, new IterativePosPIDController(0.04, 0, 0, 0, TimeUtilFactory::create()));
	#endif

	robot.arm = new ArmController(new okapi::Motor(mArm), new IterativePosPIDController(0.2, 0, 0, 0, TimeUtilFactory::create()));

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
	.withRange(-50, 3100/6)
	.withRes(300)
	.withLines(10, 8)
	.withRefresh(20)
	.withSeries("Target/6", &robot.flywheel->targetRpm, LV_COLOR_RED, 6)
	.withSeries("Current/6", &robot.flywheel->currentRpm, LV_COLOR_BLUE, 6)
	.withSeries("Power", &robot.flywheel->motorPower, LV_COLOR_GREEN)
	.withSeries("D", &robot.flywheel->pid->m_derivative, LV_COLOR_PURPLE)
	//.withSeries("Accel", &robot.flywheel->currentAccel, LV_COLOR_LIME)
	.build();

	display.odom = new OdomDisplay(display.main->newTab("Odom"), robot.tracker);
	display.angler = new AngleTuner(display.main->newTab("Angler"), robot.shooter);
	display.driverDisplay = new DriverDisplay(display.main->newTab("Driver"));

}
