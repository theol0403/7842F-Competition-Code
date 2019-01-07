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


    QLength computeAngleOfPoint(Point);
    void turnAngle(QAngle);
    void turnToAngle(QAngle);
    void turnToPoint(Point);

    QLength computeDistanceToPoint(Point);
    QLength computeDistanceOfPoint(Point);

    void driveToPoint(Point);
    void driveToPointAndAngle(Point);


  };

}
