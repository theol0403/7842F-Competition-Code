//#include "Auto/AutoSimpleFunctions.hpp"
// robotChassis->moveDistance(4_in);
// robotChassis->turnAngle(90_deg);
// setIntakeMode();   intakeCollect, intakeShoot, intakeOff, intakeOut, indexShoot
// setFlywheelRPM(2900);

//collect
//forward 40
//back 40
//90 right
//shoot 2500

setFlywheelRPM(2800);
setIntakeMode(intakeModes::loading);

chassisOdomTracker->setState(1_ft, 7_ft, 90_deg);
chassisOdomController->driveToPoint(lib7842::Point{5_in, 7_in});
chassisOdomTracker->driveToPointAndAngle(1_ft, 8_ft, 0_deg);


// setIntakeMode(intakeCollect);
// robotChassis->moveDistance(40_in);
// robotChassis->moveDistance(-40_in);
// robotChassis->turnAngle(-91_deg);
// robotChassis->moveDistance(-15_in);
// setIntakeMode(indexShoot);
// pros::delay(200);
// setIntakeMode(intakeOff);
// robotChassis->turnAngle(-4_deg);
// setIntakeMode(indexShoot);
// robotChassis->moveDistance(20_in);
