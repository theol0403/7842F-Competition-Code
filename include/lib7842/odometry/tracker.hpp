#pragma once
#include "main.h"
#include "odomMath.hpp"

namespace lib7842 {

class OdomTracker {

private:
  QLength m_lastLeft {0_in};
  QLength m_lastRight {0_in};

public:
  using trackerFunc_t = std::function<void(OdomTracker*)>;

  std::shared_ptr<okapi::SkidSteerModel> model;
  const QLength m_chassisWidth;
  const QLength m_wheelDiam;
  const double m_degToInch;
  trackerFunc_t m_trackerFunc;

  okapi::VelMath m_leftVelMath;
  okapi::VelMath m_rightVelMath;
  pros::Task m_task;

  QPoint state {0_in, 0_in, 0_rad};

  OdomTracker(std::shared_ptr<okapi::SkidSteerModel>, QLength, QLength, double, trackerFunc_t);

  void debug();

  void setState(QPoint);
  QPoint& getState();

  void setX(QLength);
  void setY(QLength);
  void setTheta(QAngle);

  QLength& getX();
  QLength& getY();
  QAngle& getTheta();

  void resetState();
  void resetSensors();
  void reset();

  double getLeftVelocity();
  double getRightVelocity();
  double getAvgVelocity();
  double getAbsLeftVelocity();
  double getAbsRightVelocity();
  double getAbsAvgVelocity();

  void run();
  static void taskFnc(void*);

  static void aTracking(OdomTracker*);
  static void mTracking(OdomTracker*);
  static void mdTracking(OdomTracker*);

private:
  void m_aTracking();
  void m_mTracking();
  void m_mdTracking();
};

} // namespace lib7842
