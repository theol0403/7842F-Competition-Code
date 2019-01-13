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

setFlywheelRPM(2500);
setIntakeMode(intakeCollect);
robotChassis->moveDistance(40_in);
robotChassis->moveDistance(-40_in);
robotChassis->turnAngle(90_deg);
setIntakeMode(indexShoot);