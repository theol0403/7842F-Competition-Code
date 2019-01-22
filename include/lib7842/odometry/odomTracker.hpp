#pragma once
#include "main.h"


namespace lib7842
{

  struct Point
  {
    QLength x {0_in};
    QLength y {0_in};
    QAngle theta {0_rad};
  };

  class OdomTracker
  {

  private:

    QLength m_lastLeftInch {0_in};
    QLength m_lastRightInch {0_in};
    QLength m_lastMiddleInch {0_in};

  public:

    const QLength m_chassisWidth;
    const QLength m_distanceMiddle;

    const double m_mainDegToInch;
    const double m_middleDegToInch;

    std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> model;

    Point state {0_in, 0_in, 0_rad};

    OdomTracker(
      std::shared_ptr<okapi::ThreeEncoderSkidSteerModel>,
      QLength, QLength,
      QLength,
      double, double
    );


    void debug();
    void step();

    void setState(QLength, QLength, QAngle);
    void setState(Point);
    void setX(QLength);
    void setY(QLength);
    void setTheta(QAngle);

    void resetState();

    void resetSensors();

    static void odometryTask(void*);

  };

}
