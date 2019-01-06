#pragma once
#include "main.h"


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

    const double m_chassisWidth;
    const double m_distanceMiddle;

    const double m_mainDegToInch;
    const double m_middleDegToInch;

    double m_lastLeftInch = 0;
    double m_lastRightInch = 0;
    double m_lastMiddleInch = 0;

  public:
    Odometry(
      okapi::ADIEncoder*,
      okapi::ADIEncoder*,
      okapi::ADIEncoder*,
      double, double,
      double, double,
      double
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
