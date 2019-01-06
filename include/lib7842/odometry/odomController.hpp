#pragma once
#include "main.h"
#include "odomTracker.hpp"


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


    double computeDistanceToPoint(OdomPoint);
    double computeAngleToPoint(OdomPoint);

    void turnToAngle(QAngle);
    void turnToPoint(OdomPoint);
    void driveToPoint(OdomPoint);
    void driveToPointAndAngle(OdomPoint);


  };

}
