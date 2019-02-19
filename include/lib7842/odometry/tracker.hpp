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

  public:

    OdomTracker (
      std::shared_ptr<okapi::SkidSteerModel>,
      QLength, QLength, double,
      std::function<void(OdomTracker*)>
    );


    std::shared_ptr<okapi::SkidSteerModel> model;
    const QLength m_chassisWidth;
    const double m_mainDegToInch;

    std::function<void(OdomTracker*)> m_trackerFunc;
    pros::Task m_trackerTask;

    qPoint state {0_in, 0_in, 0_rad};

    void debug();

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

    static void aTracking(OdomTracker*);

  private:
    
    void m_aTracking();

  };

}
