#include "DriverControl.hpp"
#include <sstream>

static okapi::ControllerButton flywheelTrigger = j_Main[ControllerDigital::B];

static ShootController::shootMacros shootMacro = ShootController::shootMacros::off;
static ShootController::shootMacros lastShootMacro = ShootController::shootMacros::off;

static IntakeController::intakeStates intakeState = IntakeController::off;
static IntakeController::intakeStates lastIntakeState = IntakeController::off;

//static okapi::ControllerButton armTrigger = j_Main[ControllerDigital::X];

void driverControl()
{

//if (Arm1.get_position()>10 || Arm1.get_position()<550){
	if (j_Digital(A)) {Arm1.move_velocity(-200);}
	else if (j_Digital(X)){ Arm1.move_velocity(200);}
//}
	else if (Arm1.get_position()<-600) {
		Arm1.move_velocity(100);
	}
	else if (Arm1.get_position()>-30) {
	Arm1.move_velocity(-100);
  }
	else{Arm1.move_velocity(0);}

std::cout <<"Arm1:"<< Arm1.get_position() << std::endl;
	/**
	* Automatic Shoot Control
	* Arrow buttons set odom y
	* Angle of hood is calculated from y using lookup table
	* Pressing one of the two shoot buttons (representing flag) will drop the hood to the proper angle and shoot
	*/
	if(j_Digital(Y)) {
		shootMacro = ShootController::shootMacros::shootOut;
	} else if(j_Digital(L2) && j_Digital(L1)) {
		shootMacro = ShootController::shootMacros::shootBoth;
	} else if(j_Digital(L2)) {
		shootMacro = ShootController::shootMacros::shootMiddle;
	} else if(j_Digital(L1)) {
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
	if(j_Digital(down))
	{
		robot.shooter->setDistanceToFlag(3.5_ft);
	}
	else if(j_Digital(left))
	{
		robot.shooter->setDistanceToFlag(4_ft);
	}
	else if(j_Digital(up))
	{
		robot.shooter->setDistanceToFlag(8.5_ft);
	}
	else if(j_Digital(right))
	{
		robot.shooter->setDistanceToFlag(11_ft);
	}

	std::stringstream distStr;
	distStr << robot.shooter->distanceToFlag.convert(foot);
	robot.printer->print(2, distStr.str() + "\' to flag");


	/**
	* Intake Control
	*/
	if(j_Digital(R2) && j_Digital(R1)) {
		intakeState = IntakeController::outSlow;
	} else if(j_Digital(R2)) {
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


	/**
	* Flywheel Toggle
	* Angling Abort
	* Arm Abort
	*/
	if(flywheelTrigger.changedToPressed()) {
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

	if((robot.flywheel->targetRpm - robot.flywheel->currentRpm) < 100) {
		robot.printer->print(1, "Flywheel Ready");
	} else {
		robot.printer->print(1, "NOT READY: " + std::to_string((int)(robot.flywheel->targetRpm - robot.flywheel->currentRpm)));
	}




	/**
	* Arm Control
	*/

	//else{robot.arm->setState(ArmController::off);}


	// if(j_Digital(A)) {
	// 	robot.arm->setState(ArmController::forward);
	// } else if(j_Digital(X)) {
	// 	robot.arm->setState(ArmController::back);
	// }else{robot.arm->setState(ArmController::off);}


}
