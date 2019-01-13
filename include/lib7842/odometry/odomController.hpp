#pragma once
#include "main.h"
#include "odomTracker.hpp"
#include "lib7842/other/miscUtils.hpp"

namespace lib7842
{

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


    QLength computeDistanceToPoint(Point);
    QAngle computeAngleToPoint(Point);

    void turnToAngle(QAngle);
    void turnToPoint(Point);
    void driveToPoint(Point);
    void driveToPointAndAngle(Point);


  };

}
