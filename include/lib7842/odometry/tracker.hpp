#pragma once
#include "main.h"
#include "odomMath.hpp"

namespace lib7842
{

  class OdomTracker
  {

  private:

    QLength m_lastLeftInch {0_in};
    QLength m_lastRightInch {0_in};
    QLength m_lastMiddleInch {0_in};

  public:

    OdomTracker (
      std::shared_ptr<okapi::ThreeEncoderSkidSteerModel>,
      QLength, QLength,
      QLength,
      double, double
    );


    std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> model;

    const QLength m_chassisWidth;
    const QLength m_distanceMiddle;

    const double m_mainDegToInch;
    const double m_middleDegToInch;

    pros::Task m_trackerTask;

    qPoint state {0_in, 0_in, 0_rad};

    void debug();
    void step();

    void setState(QLength, QLength, QAngle);
    void setState(qPoint);
    qPoint& getState();

    void setX(QLength);
    void setY(QLength);
    void setTheta(QAngle);

    QLength& getX();
    QLength& getY();
    QAngle& getTheta();

    void resetState();
    void resetSensors();

    static void odometryTask(void*);

  };

}
