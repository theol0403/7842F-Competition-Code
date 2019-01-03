#pragma once
#include "main.h"

#define WHEEL_D 41.69
#define SL 4.25 // TODO: measure all of these
#define SR 4.25
#define SS 0.55

namespace lib7842
{

  template <typename T> T toRAD(T deg)
  {
    return (deg) * (PI / 180);
  }

  template <typename T> T toDEG(T rad)
  {
    return (rad) * (180 / PI);
  }

  struct odomPoint {
    double x;
    double y;
  };

  class Odometry
  {

  private:

    okapi::ADIEncoder *m_lEncoder;
    okapi::ADIEncoder *m_rEncoder;
    okapi::ADIEncoder *m_mEncoder;

    const double m_distanceLeft;
    const double m_distanceRight;
    const double m_distanceMiddle;

    const double m_mainWheelDiam;
    const double m_middleWheelDiam;

    double m_lLastVal = 0;
    double m_rLastVal = 0;
    double m_mLastVal = 0;

  public:
    Odometry(
      okapi::ADIEncoder*,
      okapi::ADIEncoder*,
      okapi::ADIEncoder*,
      double, double, double,
      double, double
    );

    // for ease of use we make the x, y, and a vars public
    double xPos = 0;
    double yPos = 0;
    double aPos = 0;

    void debug();
    void step();

    void setPos(double x, double y, double a);
    void resetSensors();

  };



}
