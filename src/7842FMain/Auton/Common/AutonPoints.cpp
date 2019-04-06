#include "7842FMain/Auton/AutonIncludes.hpp"

/*
 * These only represent the left (red) side of the court
 * It is assumed they will mirror perfectly along the middle of the court
 */

QLength startX {2_in}; //The X of the robot in the setup position

qPoint sideFlagShoot {1.35_ft, 11.2_ft}; //The shooting target for the flag
qPoint sideFlagPost {2_ft, 11_ft}; //The flag post

qPoint middleFlagShoot {5.45_ft, 11.2_ft}; //The shooting target for the flag
qPoint middleFlagPost {6_ft, 11_ft}; //The flag post

qPoint closeFlatCap {3_ft, 9_ft}; //The close cap with the balls on it

qPoint sidePlatform {4.2_ft, 5_ft}; //The driving position to get ready to climb platform
