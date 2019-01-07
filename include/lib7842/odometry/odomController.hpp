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

    std::shared_ptr<okapi::ChassisControllerPID> m_chassisController;
    OdomTracker *m_odomTracker;

  public:
    OdomController(
      std::shared_ptr<okapi::ChassisControllerPID>,
      OdomTracker*
    );


    double computeDistanceToPoint(Point);
    double computeAngleToPoint(Point);

    void turnToAngle(QAngle);
    void turnToPoint(Point);
    void driveToPoint(Point);
    void driveToPointAndAngle(Point);


  };

}
