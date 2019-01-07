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

  public:

    OdomController(
      OdomTracker*,
      std::unique_ptr<IterativePosPIDController>,
      std::unique_ptr<IterativePosPIDController>,
      std::unique_ptr<IterativePosPIDController>
    );


    QLength computeDistanceToPoint(Point);
    QAngle computeAngleToPoint(Point);

    void turnToAngle(QAngle);
    void turnToPoint(Point);
    void driveToPoint(Point);
    void driveToPointAndAngle(Point);


  };

}
