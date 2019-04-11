#include "7842FMain/DriverControl.hpp"
#include <sstream>

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

void driverControl()
{

	/**
	* Base Control
	*/
	double rightY = mAnalog(RIGHT_Y);
	double leftX = mAnalog(LEFT_X);
	subsystem(model)->arcade(rightY, ipow(std::abs(leftX), 3) * sgn(leftX), 0);

	/**
	* Intake Control
	*/
	if(mDigital(R2) && mDigital(R1)) {
		intakeState = IntakeController::outSlow;
	} else if(mDigital(R2) || pDigital(R2)) {
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
	if(mDigitalPressed(Y)) {
		if(robot.arm->getState() == ArmController::down) {
			robot.arm->setState(ArmController::aboveWall);
		} else if(robot.arm->getState() == ArmController::aboveWall) {
			robot.arm->setState(ArmController::descore);
		} else {
			robot.arm->setState(ArmController::down);
		}
	}

	/**
	* Flywheel Toggle
	* Angling Abort
	* Arm Abort
	*/
	if(mDigitalPressed(B) || pDigitalPressed(B)) {
		robot.mPrinter->rumble("-");
		if(robot.flywheel->getTargetRpm() == 0) {
			robot.flywheel->setRpm(globalFlywheelRPM);
		} else {
			robot.flywheel->setRpm(0);
		}

		robot.arm->setState(ArmController::off);
	}

	/**
	* Flywheel Ready Printing
	*/
	if((robot.flywheel->targetRpm - robot.flywheel->currentRpm) < 50) {
		robot.mPrinter->print(1, "Flywheel Ready");
		display.driverDisplay->setColor(LV_COLOR_LIME);
	} else {
		robot.mPrinter->print(1, "NOT READY: " + std::to_string((int)(robot.flywheel->targetRpm - robot.flywheel->currentRpm)));
		display.driverDisplay->setColor(LV_COLOR_MAGENTA);
	}


	/**
	* Distance Control
	*/
	if(mDigital(DOWN)) {
		robot.shooter->setDistanceToFlag(3.5_ft);
	} else if(mDigital(LEFT)) {
		robot.shooter->setDistanceToFlag(4.5_ft);
	} else if(mDigital(UP)) {
		robot.shooter->setDistanceToFlag(8.5_ft);
	} else if(mDigital(RIGHT)) {
		robot.shooter->setDistanceToFlag(11_ft);
	}

	std::stringstream distStr;
	distStr << robot.shooter->distanceToFlag.convert(foot);
	robot.mPrinter->print(2, distStr.str() + "\' to flag");



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



	static ShootController::shootMacros wantedMacro = ShootController::shootMacros::off;
	static ShootController::shootMacros lastWantedMacro = ShootController::shootMacros::off;

	//set wanted action
	if(pDigital(X)) {
		shootMacro = ShootController::shootMacros::shootOut;
	} else if(pDigital(L2) && pDigital(L1)) {
		shootMacro = ShootController::shootMacros::shootBoth;
	} else if(pDigital(L2)) {
		shootMacro = ShootController::shootMacros::shootMiddle;
	} else if(pDigital(L1)) {
		shootMacro = ShootController::shootMacros::shootTop;
	} else {
		wantedMacro = ShootController::shootMacros::off;
	}

	if(pDigitalPressed(R1)) {
		robot.shooter->doJob(wantedMacro);
	} else {

		//if wanted changes
		if(wantedMacro != lastWantedMacro) {

			//if it is not off, just do the angling
			if(wantedMacro != ShootController::shootMacros::off) {
				switch(wantedMacro) {

					case ShootController::shootMacros::shootOut : {
						robot.shooter->doJob(ShootController::angleOut);
					}

					case ShootController::shootMacros::shootBoth : {
						robot.shooter->doJob(ShootController::angleTop);
					}

					case ShootController::shootMacros::shootMiddle : {
						robot.shooter->doJob(ShootController::angleMiddle);
					}

					case ShootController::shootMacros::shootTop : {
						robot.shooter->doJob(ShootController::angleTop);
					}
				}

			} else { //if it is off, standby (cycle)
				robot.shooter->doJob(ShootController::standby);
			}

			//update last
			lastWantedMacro = wantedMacro;

		}

	}


















}
