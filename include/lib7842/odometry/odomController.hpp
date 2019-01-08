#pragma once
#include "main.h"
#include "odomTracker.hpp"

namespace lib7842
{

  class OdomController
  {

  private:

    OdomTracker *m_chassis;

    std::unique_ptr<IterativePosPIDController> m_distancePid;
    std::unique_ptr<IterativePosPIDController> m_anglePid;
    std::unique_ptr<IterativePosPIDController> m_turnPid;

    SettledUtil m_distanceUtil;
    SettledUtil m_angleUtil;
    SettledUtil m_turnUtil;

  public:

    OdomController(
      OdomTracker*,
      std::unique_ptr<IterativePosPIDController>,
      std::unique_ptr<IterativePosPIDController>,
      std::unique_ptr<IterativePosPIDController>
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
