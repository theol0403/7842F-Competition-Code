#include "odomTracker.hpp"

namespace lib7842
{

  OdomTracker::OdomTracker
  (
    okapi::ADIEncoder *lEncoder,
    okapi::ADIEncoder *rEncoder,
    okapi::ADIEncoder *mEncoder,
    double chassisWidth, double distanceMiddle,
    double mainTicksPerRev, double middleTicksPerRev,
    double wheelDiam
  ):
  m_lEncoder(lEncoder),
  m_rEncoder(rEncoder),
  m_mEncoder(mEncoder),

  m_chassisWidth(chassisWidth),
  m_distanceMiddle(distanceMiddle),

  m_mainDegToInch(wheelDiam * PI / mainTicksPerRev),
  m_middleDegToInch(wheelDiam * PI / middleTicksPerRev)
  {

  };


  void OdomTracker::debug()
  {
    printf("x: %1.2f, y: %1.2f, theta: %1.2f\n",
    trackingPoint.x,
    trackingPoint.y,
    toDEG(trackingPoint.theta));
  }

  void OdomTracker::step()
  {

    double newLeftInch = m_lEncoder->get() * m_mainDegToInch;
    double newRightInch = m_rEncoder->get() * m_mainDegToInch;
    double newMiddleInch = m_mEncoder->get() * m_middleDegToInch;

    double dLeftInch = newLeftInch - m_lastLeftInch;
    double dRightInch = newRightInch - m_lastRightInch;
    double dMiddleInch = newMiddleInch - m_lastMiddleInch;

    m_lastLeftInch = newLeftInch;
    m_lastRightInch = newRightInch;
    m_lastMiddleInch = newMiddleInch;

    double newAngle = (newLeftInch - newRightInch) / m_chassisWidth;
    double dAngle = newAngle - trackingPoint.theta;

    // std::cout << "dA: " << dA << std::endl;

    double dAvgMainInch = (dLeftInch + dRightInch) / 2;

    double localOffX = 0, localOffY = 0;

    if(dAngle == 0.0)
    {
      localOffX = dMiddleInch;
      localOffY = dAvgMainInch;
    }
    else
    {
      localOffX = 2 * sin(dAngle / 2) * ((dMiddleInch / dAngle) + m_distanceMiddle);
      localOffY = 2 * sin(dAngle / 2) * ((dAvgMainInch / dAngle) + (m_chassisWidth / 2));
    }

    double avgAngle = trackingPoint.theta + (dAngle / 2);

    double polarR = sqrt((localOffX * localOffX) + (localOffY * localOffY));
    double polarA = atan2(localOffY, localOffX) - avgAngle;

    double dX = cos(polarA) * polarR;
    double dY = sin(polarA) * polarR;

    if(dX > 20) {
      lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
      lv_label_set_text(label, "stop everything has gone horribly wrong X");
      lv_obj_align(label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 5);
    }

    if(dY > 20) {
      lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
      lv_label_set_text(label, "stop everything has gone horribly wrong Y");
      lv_obj_align(label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 5);
    }


    trackingPoint.x += dX;
    trackingPoint.y += dY;
    trackingPoint.theta = newAngle;

  }

  void OdomTracker::setPos(double x, double y, double a)
  {
    trackingPoint.x = x;
    trackingPoint.y = y;
    trackingPoint.theta = a;
  }

  void OdomTracker::setPos(OdomPoint point)
  {
    trackingPoint.x = point.x;
    trackingPoint.y = point.y;
    trackingPoint.theta = point.theta;
  }

  void OdomTracker::resetSensors()
  {
    m_lEncoder->reset();
    m_rEncoder->reset();
    m_mEncoder->reset();
    m_lastLeftInch = 0;
    m_lastRightInch = 0;
    m_lastMiddleInch = 0;
  }



  void OdomTracker::odometryTask(void* odometryPtr)
  {
    OdomTracker* thisOdomTracker = static_cast<OdomTracker*>(odometryPtr);
    std::cout << "Begin Tracking" << std::endl;

    int count = 0;
    while(true)
    {
      thisOdomTracker->step();

      count++;
      if(count > 20)
      {
        thisOdomTracker->debug();
        count = 0;
      }

      pros::delay(10);
    }
  }


}
