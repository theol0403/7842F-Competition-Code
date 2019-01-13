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
setIntakeMode(intakeCollect);
robotChassis->moveDistance(40_in);
robotChassis->moveDistance(-40_in);
robotChassis->turnAngle(91_deg);
robotChassis->moveDistance(-13_in);
setIntakeMode(indexShoot);
pros::delay(200);
setIntakeMode(intakeOff);
robotChassis->moveDistance(23_in);
robotChassis->turnAngle(4_deg);
setIntakeMode(indexShoot);
robotChassis->moveDistance(20_in);