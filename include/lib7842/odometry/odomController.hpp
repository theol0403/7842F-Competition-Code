#pragma once
#include "main.h"
#include "odomTracker.hpp"
#include "lib7842/pid/pidSystem.hpp"

namespace lib7842
{

  class OdomController
  {

  private:

    OdomTracker *m_chassis;

    PID *m_distancePid;
    PID *m_anglePid;
    PID *m_turnPid;

  public:

    OdomController(
      OdomTracker*,
      PID *,
      PID *,
      PID *
    );


    QAngle computeAngleToPoint(Point);
    void turnAngle(QAngle);
    void turnToAngle(QAngle);
    void turnToPoint(Point);

    QLength computeDistanceToPoint(Point);
    QLength computeDistanceBetweenPoints(Point, Point);

    void driveToPoint(Point);
    void driveToPointAndAngle(Point);


  };

}
