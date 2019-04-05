#include "lib7842/odometry/odomMath.hpp"
using namespace lib7842;

/**
 * Points
 */
extern QTime shootTime;
extern QLength startX;

extern qPoint sideFlagShoot;
extern qPoint sideFlagPost;

extern qPoint middleFlagShoot;
extern qPoint middleFlagPost;

extern qPoint closeFlatCap;

extern qPoint sidePlatform;


/**
 * Actions
 */
void firstCapMovement(SideController& chassis, QLength y);
void waitForLastMinute(Timer& timer);
