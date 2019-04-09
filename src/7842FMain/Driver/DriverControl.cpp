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
	} else if(mDigital(R2)) {
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

}
