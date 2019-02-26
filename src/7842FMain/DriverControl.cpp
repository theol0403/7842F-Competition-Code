#include "DriverControl.hpp"

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

static ArmController::armStates armState = ArmController::out;
static ArmController::armStates lastArmState = ArmController::out;

enum class driverShootModes {
	automatic,
	manual
};

driverShootModes driverShootMode = driverShootModes::automatic;

static okapi::ControllerButton manualTrigger = j_Main[ControllerDigital::down];

static ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;

static double targetAngle = 0;


void driverControl()
{
	/**
	* Intake Control
	*/
	if(j_Digital(R2)) {
		intakeState = IntakeController::intakeBall;
	} else if(j_Digital(R1)) {
		intakeState = IntakeController::outIntake;
	} else {
		intakeState = IntakeController::off;
	}

	if(intakeState != lastIntakeState) {
		robot.intake->setState(intakeState);
		lastIntakeState = intakeState;
	}


	if(j_Digital(B)) {
		armState = ArmController::down;
	} else if(j_Digital(Y)) {
		armState = ArmController::out;
	} else if(j_Digital(X)) {
		armState = ArmController::up;
	} else {
		//armState = ArmController::out;
	}

	if(armState != lastArmState) {
		robot.arm->setState(armState);
		lastArmState = armState;
	}

	/**
	* Switches Shooting Mode
	* Turns on flywheel
	*/
	if(manualTrigger.changedToPressed()) {
		if(robot.flywheel->getTargetRpm() == 0) {
			robot.flywheel->setRpm(globalFlywheelRPM);
			robot.arm->setState(ArmController::unfold); //for testing
		} else {
			if(driverShootMode == driverShootModes::automatic) {
				driverShootMode = driverShootModes::manual;
			} else {
				driverShootMode = driverShootModes::automatic;
			}
		}
		shootMacro = ShootController::shootMacros::off;
		lastShootMacro = ShootController::shootMacros::off;
		robot.shooter->doJob(ShootController::off);
		robot.arm->setState(ArmController::off);
	}

	/**
	* Automatic Control
	* Arrow buttons set odom y
	* Angle of hood is calculated from y using lookup table
	* Pressing one of the two shoot buttons (representing flag) will drop the hood to the proper angle and shoot
	*/
	if(driverShootMode == driverShootModes::automatic)
	{
		/**
		* Shoot Control
		*/
		if(j_Digital(L2) && j_Digital(L1)) {
			shootMacro = ShootController::shootMacros::shootBothFlags;
		} else if(j_Digital(L2)) {
			shootMacro = ShootController::shootMacros::shootMiddleFlag;
		} else if(j_Digital(L1)) {
			//shootMacro = ShootController::shootMacros::shootTopFlag;
			shootMacro = ShootController::shootMacros::shootTarget;
		} else {
			shootMacro = ShootController::shootMacros::off;
		}

		if(shootMacro != lastShootMacro)
		{
			if(shootMacro == ShootController::shootMacros::shootTarget) std::cout << "Shot at Y : " << robot.tracker->state.y.convert(foot) << " at Angle : " << targetAngle << std::endl;
			if(shootMacro != ShootController::shootMacros::off) robot.shooter->doMacro(shootMacro);
			lastShootMacro = shootMacro;
		}


		/**
		* Angle Control
		*/
		// if(j_Digital(Y))
		// {
		// 	targetAngle -= 1;
		// 	robot.shooter->setTarget(targetAngle);
		// 	std::cout << "Target Angle: " << targetAngle << std::endl;
		// }
		// else if(j_Digital(X))
		// {
		// 	targetAngle += 1;
		// 	robot.shooter->setTarget(targetAngle);
		// 	std::cout << "Target Angle: " << targetAngle << std::endl;
		// }


		if(j_Digital(left))
		{
			robot.tracker->setY(9_ft);
			robot.tracker->setTheta(0_deg);
		}
		else if(j_Digital(up))
		{
			robot.tracker->setY(6_ft);
			robot.tracker->setTheta(0_deg);
		}
		else if(j_Digital(right))
		{
			robot.tracker->setY(0_ft);
			robot.tracker->setTheta(0_deg);
		}

		robot.shooter->setDistanceToFlag(11_ft - robot.tracker->state.y);
	}

	/**
	* Manual Control
	* L1 Controls angle
	* L2 Shoots
	*/
	else
	{
		/**
		* Shoot Controller
		* Manual Angling
		*/
		if(j_Digital(L2))
		{
			shootMacro = ShootController::shootMacros::shoot;
		}
		else if(j_Digital(L1))
		{
			shootMacro = ShootController::shootMacros::angleManual;
		}
		else
		{
			shootMacro = ShootController::shootMacros::off;
		}

		if(shootMacro != lastShootMacro)
		{
			robot.shooter->doMacroLoop(shootMacro);
			lastShootMacro = shootMacro;
		}
	}

}
