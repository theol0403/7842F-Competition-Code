#pragma once
#include "main.h"

#include "7842FMain/MotorConfig.hpp"
#include "lib7842/other/miscUtils.hpp"

#include "7842FMain/Shared/IntakeTask.hpp"
#include "7842FMain/Shared/FlywheelTask.hpp"

void AutoRedClose();
void AutoBlueClose();

void AutoRedFar();
void AutoBlueFar();

void AutoRedMiddle();
void AutoBlueMiddle();


lib7842::Point leftFlag{0.8_ft, 11.5_ft};
