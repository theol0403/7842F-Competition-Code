#pragma once
#include "main.h"
#include "odomTracker.hpp"

namespace lib7842
{

  struct Point
  {
    QLength x {0_in};
    QLength y {0_in};
    QAngle theta {0_rad};
  };

  class OdomController
  {

  private:

    OdomTracker *m_odom;

    std::unique_ptr<IterativePosPIDController> m_distancePid;
    std::unique_ptr<IterativePosPIDController> m_turnPid;
    std::unique_ptr<IterativePosPIDController> m_anglePid;

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
