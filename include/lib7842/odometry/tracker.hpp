#pragma once
#include "main.h"
#include "odomMath.hpp"

namespace lib7842
{

  class OdomTracker
  {

  private:

    QLength m_lastLeft {0_in};
    QLength m_lastRight {0_in};

  public:

    std::shared_ptr<okapi::SkidSteerModel> model;
    const QLength m_chassisWidth;
    const QLength m_wheelDiam;
    const double m_degToInch;
    std::function<void(OdomTracker*)> m_trackerFunc;
    pros::Task m_task;

    qPoint state {0_in, 0_in, 0_rad};

    OdomTracker (
      std::shared_ptr<okapi::SkidSteerModel>,
      QLength, QLength, double,
      std::function<void(OdomTracker*)>
    );

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
    void reset();

    static void taskFnc(void*);

    static void aTracking(OdomTracker*);
    static void mTracking(OdomTracker*);
    static void mdTracking(OdomTracker*);

  private:

    void m_aTracking();
    void m_mTracking();
    void m_mdTracking();

  };

}
