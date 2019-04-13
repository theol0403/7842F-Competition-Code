#include "7842FMain/Auton/AutonIncludes.hpp"

/*
 * These only represent the left (red) side of the court
 * It is assumed they will mirror perfectly along the middle of the court
 */

QLength startX {9_in}; //The X of the robot in the setup position

QPoint sideFlagShoot {1.35_ft, 11.2_ft}; //The shooting target for the flag
QPoint sideFlagPost {2_ft, 11_ft}; //The flag post

QPoint middleFlagShoot {5.45_ft, 11.2_ft}; //The shooting target for the flag
QPoint middleFlagPost {6_ft, 11_ft}; //The flag post

QPoint farFlagShoot {8.8_ft, 11.2_ft}; //The shooting target for the flag
QPoint farFlagPost {10_ft, 11_ft}; //The flag post

QPoint closeFlatCap {3_ft, 9_ft}; //The close cap with the balls on it
QPoint farFlatCap {5_ft, 1_ft}; //The far cap with the balls on it

QPoint sidePlatform {4.2_ft, 5_ft}; //The driving position to get ready to climb platform
