#include "main.h"
#include "Include/Shared/MotorConfig.hpp"
#include "../src/Libraries/PID/VelPIDSystem.hpp"

extern bool extendArm;

void setFlywheelRPM(int);
void flywheelTask(void*);
