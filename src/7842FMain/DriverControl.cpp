#include "7842FMain/DriverControl.hpp"
#include <sstream>

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

static ArmController::armStates armState = ArmController::hold;
static ArmController::armStates lastArmState = ArmController::hold;

static ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;

void driverControl()
{

	/**
	* Base Control
	*/
	double rightY = mAnalog(RIGHT_Y);
	double leftX = mAnalog(LEFT_X);
	if(mDigital(RIGHT)) {
		robot.model->arcade(rightY, (ipow(std::abs(leftX), 2) * sgn(leftX)) + robot.vision->allign(), 0);
	} else {
		robot.model->arcade(rightY, ipow(std::abs(leftX), 2) * sgn(leftX), 0);
	}

	/**
	* Intake Control
	*/
	if(mDigital(R2) && mDigital(R1)) {
		intakeState = IntakeController::outSlow;
	} else if(mDigital(R2) || pDigital(R2) || pDigital(R1)) {
		intakeState = IntakeController::intakeBall;
	} else if(mDigital(R1)) {
		intakeState = IntakeController::outIntake;
	} else {
		intakeState = IntakeController::off;
	}

	if(intakeState != lastIntakeState) {
		robot.intake->setState(intakeState);
		lastIntakeState = intakeState;
	}

	/**
	* Arm Control
	*/
	if(mDigital(DOWN)) {
		armState = ArmController::carry;
	} else if(mDigital(L1) && mDigital(L2)) {
		armState = ArmController::upSlow;
	} else if(mDigital(L1)) {
		armState = ArmController::up;
	} else if(mDigital(L2)) {
		armState = ArmController::down;
	} else {
		if(armState != ArmController::carry) {
			armState = ArmController::hold;
		}
	}

	if(armState != lastArmState) {
		robot.arm->setState(armState);
		lastArmState = armState;
	}

	/**
	* Flywheel Toggle
	* Angling Abort
	* Arm Abort
	*/
	if(mDigitalPressed(B) || pDigitalPressed(B)) {
		robot.printer->rumble(".");
		if(robot.flywheel->getTargetRpm() == 0) {
			robot.flywheel->setRpm(globalFlywheelRPM);
		} else {
			robot.flywheel->setRpm(0);
		}
		shootMacro = ShootController::shootMacros::off;
		lastShootMacro = ShootController::shootMacros::off;
		robot.shooter->doJob(ShootController::off);

		//robot.arm->setState(ArmController::off);
	}

	/**
	* Flywheel Ready Printing
	*/
	if((robot.flywheel->targetRpm - robot.flywheel->currentRpm) < 50) {
		robot.printer->print(1, "Flywheel Ready");
		display.driverDisplay->setColor(LV_COLOR_LIME);
	} else {
		robot.printer->print(1, "NOT READY: " + std::to_string((int)(robot.flywheel->targetRpm - robot.flywheel->currentRpm)));
		display.driverDisplay->setColor(LV_COLOR_MAGENTA);
	}


	/***
	*      ___              _ _
	*     / _ \            | (_)
	*    / /_\ \_ __   __ _| |_ _ __   __ _
	*    |  _  | '_ \ / _` | | | '_ \ / _` |
	*    | | | | | | | (_| | | | | | | (_| |
	*    \_| |_/_| |_|\__, |_|_|_| |_|\__, |
	*                  __/ |           __/ |
	*                 |___/           |___/
	*/

	/**
	* Distance Control
	*/
	if(pDigital(DOWN)) {
		robot.shooter->setDistanceToFlag(3.5_ft);
		lastShootMacro = ShootController::shootMacros::off;
	} else if(pDigital(LEFT)) {
		robot.shooter->setDistanceToFlag(4.5_ft);
		lastShootMacro = ShootController::shootMacros::off;
	} else if(pDigital(UP)) {
		robot.shooter->setDistanceToFlag(8.5_ft);
		lastShootMacro = ShootController::shootMacros::off;
	} else if(pDigital(RIGHT)) {
		robot.shooter->setDistanceToFlag(11_ft);
		lastShootMacro = ShootController::shootMacros::off;
	}

	//set wanted action
	if(pDigital(L1) && pDigital(L2)) {
		shootMacro = ShootController::shootMacros::shootBoth;
	} else if(pDigital(L1)) {
		shootMacro = ShootController::shootMacros::shootTop;
	} else if(pDigital(L2)) {
		shootMacro = ShootController::shootMacros::shootMiddle;
	} else if(pDigital(Y) && pDigital(X)) {
		shootMacro = ShootController::shootMacros::shootBothPlatform;
	} else if(pDigital(Y)) {
		shootMacro = ShootController::shootMacros::shootTopPlatform;
	} else if(pDigital(X)) {
		shootMacro = ShootController::shootMacros::shootMiddlePlatform;
	} else if(pDigital(A)) {
		shootMacro = ShootController::shootMacros::shootOut;
	}

	//cycle, then wait for button released, then go back to action
	// if(pDigitalPressed(Y)) {
	// 	robot.shooter->doMacro(ShootController::shootMacros::cycle);
	// 	//cause it to go until pressed unless trigger pressed
	// 	shootMacro = ShootController::shootMacros::off;
	// 	lastShootMacro = ShootController::shootMacros::off;
	// } else
	if(pDigitalPressed(R1)) {
		if(shootMacro == ShootController::shootMacros::off) {
			robot.shooter->doMacro(ShootController::shootMacros::shoot);
		} else {
			robot.shooter->doMacro(shootMacro);
		}
		//when completed it will go back to other action
		lastShootMacro = ShootController::shootMacros::off;
	} else if(!robot.shooter->macroCompleted) { //not completed
		//nothing
	} else {
		if(shootMacro != lastShootMacro) {
			switch(shootMacro) {
				case ShootController::shootMacros::shootBoth : {
					robot.shooter->doJob(ShootController::angleTop);
					break;
				} case ShootController::shootMacros::shootTop : {
					robot.shooter->doJob(ShootController::angleTop);
					break;
				} case ShootController::shootMacros::shootMiddle : {
					robot.shooter->doJob(ShootController::angleMiddle);
					break;
				} case ShootController::shootMacros::shootBothPlatform : {
					robot.shooter->doJob(ShootController::angleTopPlatform);
					break;
				} case ShootController::shootMacros::shootTopPlatform : {
					robot.shooter->doJob(ShootController::angleTopPlatform);
					break;
				} case ShootController::shootMacros::shootMiddlePlatform : {
					robot.shooter->doJob(ShootController::angleMiddlePlatform);
					break;
				} case ShootController::shootMacros::shootOut : {
					robot.shooter->doJob(ShootController::angleOut);
					break;
				} default: {
					std::cerr << "DriverControl: Invalid shootMacro" << std::endl;
					break;
				}
			}
			lastShootMacro = shootMacro;
		}
	}

	std::string flagString;
	switch(shootMacro) {
		case ShootController::shootMacros::shootBoth : flagString = "both"; break;
		case ShootController::shootMacros::shootTop : flagString = "top"; break;
		case ShootController::shootMacros::shootMiddle : flagString = "middle"; break;
		case ShootController::shootMacros::shootBothPlatform : flagString = "bothPlat"; break;
		case ShootController::shootMacros::shootTopPlatform : flagString = "topPlat"; break;
		case ShootController::shootMacros::shootMiddlePlatform : flagString = "middlePlat"; break;
		case ShootController::shootMacros::shootOut : flagString = "out"; break;
		default: flagString = "none"; break;
	}
	std::stringstream distStr;
	distStr << robot.shooter->distanceToFlag.convert(foot);
	robot.printer->print(2, distStr.str() + "\' to " + flagString, pros::E_CONTROLLER_MASTER);

}
