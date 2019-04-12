#include "7842FMain/DriverControl.hpp"
#include <sstream>

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

static ShootController::shootMacros wantedMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastWantedMacro = ShootController::shootMacros::off;

void driverControl()
{

	/**
	* Base Control
	*/
	double rightY = mAnalog(RIGHT_Y);
	double leftX = mAnalog(LEFT_X);
	robot.model->arcade(rightY, ipow(std::abs(leftX), 3) * sgn(leftX), 0);

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
	if(pDigital(DOWN)) {
		robot.shooter->setDistanceToFlag(3.5_ft);
		lastWantedMacro = ShootController::shootMacros::nothing;
	} else if(pDigital(LEFT)) {
		robot.shooter->setDistanceToFlag(4.5_ft);
		lastWantedMacro = ShootController::shootMacros::nothing;
	} else if(pDigital(UP)) {
		robot.shooter->setDistanceToFlag(8.5_ft);
		lastWantedMacro = ShootController::shootMacros::nothing;
	} else if(pDigital(RIGHT)) {
		robot.shooter->setDistanceToFlag(11_ft);
		lastWantedMacro = ShootController::shootMacros::nothing;
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

	//set wanted action
	if(pDigital(X)) {
		wantedMacro = ShootController::shootMacros::shootOut;
	} else if(pDigital(L2) && pDigital(L1)) {
		wantedMacro = ShootController::shootMacros::shootBoth;
	} else if(pDigital(L2)) {
		wantedMacro = ShootController::shootMacros::shootMiddle;
	} else if(pDigital(L1)) {
		wantedMacro = ShootController::shootMacros::shootTop;
	} else {
		wantedMacro = ShootController::shootMacros::off;
	}

	//when button first pressed
	if(pDigitalPressed(Y)) {
		robot.shooter->doJob(ShootController::standby);
		robot.shooter->macroCompleted = false;
	} else if(pDigitalPressed(R1)) {
		robot.shooter->doMacro(wantedMacro);
		robot.shooter->macroCompleted = false;
		lastWantedMacro = ShootController::shootMacros::nothing;
	} else if((pDigital(R1) || pDigital(Y)) && !robot.shooter->macroCompleted) { //when button held and it is still going
		//nothing
	} else {
		//if wanted changes
		if(wantedMacro != lastWantedMacro) {

			//if it is not off, just do the angling
			if(wantedMacro != ShootController::shootMacros::off) {
				switch(wantedMacro) {

					case ShootController::shootMacros::shootOut : {
						robot.shooter->doJob(ShootController::angleOut);
						break;
					}

					case ShootController::shootMacros::shootBoth : {
						robot.shooter->doJob(ShootController::angleTop);
						break;
					}

					case ShootController::shootMacros::shootMiddle : {
						robot.shooter->doJob(ShootController::angleMiddle);
						break;
					}

					case ShootController::shootMacros::shootTop : {
						robot.shooter->doJob(ShootController::angleTop);
						break;
					}

					default: {
						std::cerr << "DriverControl: Invalid wantedMacro" << std::endl;
						break;
					}
				}

			} else { //if it is off, standby (cycle)
				robot.shooter->doJob(ShootController::off);
			}

			lastWantedMacro = wantedMacro;
		}
	}


}
