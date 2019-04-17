#include "7842FMain/AutonIncludes.hpp"

/*
 * These only represent the left (red) side of the court
 * It is assumed they will mirror perfectly along the middle of the court
 */

QLength startX {9_in}; //The X of the robot in the setup position

QPoint sideFlagShoot {1.3_ft, 11_ft}; //The shooting target for the flag
QPoint sideFlagPost {2_ft, 11_ft}; //The flag post

QPoint middleFlagShoot {5.4_ft, 11_ft}; //The shooting target for the flag
QPoint middleFlagPost {6_ft, 11_ft}; //The flag post

QPoint farFlagShoot {9.2_ft, 11_ft}; //The shooting target for the flag
QPoint farFlagPost {10_ft, 11_ft}; //The flag post

QPoint closeFlatCap {3_ft, 9_ft}; //The close cap with the balls on it
QPoint farFlatCap {4.6_ft, 1_ft}; //The far cap with the balls on it

QPoint farPost {2.15_ft, 0_ft}; //the post at the end of the court
