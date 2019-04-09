#include "7842FMain/DriverControl.hpp"
#include <sstream>

static okapi::ControllerButton flywheelTrigger = j_Main[ControllerDigital::B];
//static okapi::ControllerButton partnerFlywheelTrigger = j_Partner[ControllerDigital::B];

static ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;

void angleControl()
{
	/**
	* Flywheel Toggle
	* Angling Abort
	* Arm Abort
	*/
	if(flywheelTrigger.changedToPressed()){// || partnerFlywheelTrigger.changedToPressed()) {
		robot.mPrinter->rumble("-");
		if(robot.flywheel->getTargetRpm() == 0) {
			robot.flywheel->setRpm(globalFlywheelRPM);
		} else {
			robot.flywheel->setRpm(0);
		}
		shootMacro = ShootController::shootMacros::off;
		lastShootMacro = ShootController::shootMacros::off;
		robot.shooter->doJob(ShootController::off);

		robot.arm->setState(ArmController::off);
	}

	if((robot.flywheel->targetRpm - robot.flywheel->currentRpm) < 50) {
		robot.mPrinter->print(1, "Flywheel Ready");
		display.driverDisplay->setColor(LV_COLOR_LIME);
	} else {
		robot.mPrinter->print(1, "NOT READY: " + std::to_string((int)(robot.flywheel->targetRpm - robot.flywheel->currentRpm)));
		display.driverDisplay->setColor(LV_COLOR_MAGENTA);
	}

	/**
	* Automatic Shoot Control
	* Arrow buttons set odom y
	* Angle of hood is calculated from y using lookup table
	* Pressing one of the two shoot buttons (representing flag) will drop the hood to the proper angle and shoot
	*/
	if(mDigital(X)) {
		shootMacro = ShootController::shootMacros::shootOut;
	} else if(mDigital(L2) && mDigital(L1)) {
		shootMacro = ShootController::shootMacros::shootBoth;
	} else if(mDigital(L2)) {
		shootMacro = ShootController::shootMacros::shootMiddle;
	} else if(mDigital(L1)) {
		shootMacro = ShootController::shootMacros::shootTop;
	} else {
		shootMacro = ShootController::shootMacros::off;
	}

	if(shootMacro != lastShootMacro)
	{
		//if X is released while it is angling, it will shoot
		if(lastShootMacro == ShootController::shootMacros::shootOut && shootMacro == ShootController::shootMacros::off && robot.shooter->getCurrentJob() == ShootController::angleOut) {
			robot.shooter->doMacro(ShootController::shootMacros::shoot);
		} else {
			robot.shooter->doMacro(shootMacro);
		}

		lastShootMacro = shootMacro;
	}

	/**
	* Angle Control
	*/
	if(mDigital(down)) {
		robot.shooter->setDistanceToFlag(3.5_ft);
	} else if(mDigital(left)) {
		robot.shooter->setDistanceToFlag(4.5_ft);
	} else if(mDigital(up)) {
		robot.shooter->setDistanceToFlag(8.5_ft);
	} else if(mDigital(right)) {
		robot.shooter->setDistanceToFlag(11_ft);
	}

	std::stringstream distStr;
	distStr << robot.shooter->distanceToFlag.convert(foot);
	robot.mPrinter->print(2, distStr.str() + "\' to flag");

	//std::cout << pros::c::controller_get_digital(CONTROLLER_PARTNER, DIGITAL_RIGHT) << std::endl;

}
