#include "odomTracker.hpp"

namespace lib7842
{

  OdomTracker::OdomTracker
  (
    okapi::ADIEncoder *lEncoder,
    okapi::ADIEncoder *rEncoder,
    okapi::ADIEncoder *mEncoder,
    QLength chassisWidth, QLength distanceMiddle,
    QLength wheelDiam,
    double mainTicksPerRev, double middleTicksPerRev
  ):
  m_lEncoder(lEncoder),
  m_rEncoder(rEncoder),
  m_mEncoder(mEncoder),

  m_chassisWidth(chassisWidth),
  m_distanceMiddle(distanceMiddle),

  m_mainDegToInch(wheelDiam.convert(inch) * 1_pi / mainTicksPerRev),
  m_middleDegToInch(wheelDiam.convert(inch) * 1_pi / middleTicksPerRev)
  {

  };


  void OdomTracker::debug()
  {
    printf(
      "x: %1.2f, y: %1.2f, theta: %1.2f\n",
      state.x.convert(inch),
      state.y.convert(inch),
      state.theta.convert(degree)
    );
  }

  void OdomTracker::step()
  {

    QLength newLeftInch = (m_lEncoder->get() * m_mainDegToInch) * inch;
    QLength newRightInch = (m_rEncoder->get() * m_mainDegToInch) * inch;
    QLength newMiddleInch = (m_mEncoder->get() * m_middleDegToInch) * inch;

    QLength dLeftInch = newLeftInch - m_lastLeftInch;
    QLength dRightInch = newRightInch - m_lastRightInch;
    QLength dMiddleInch = newMiddleInch - m_lastMiddleInch;

    m_lastLeftInch = newLeftInch;
    m_lastRightInch = newRightInch;
    m_lastMiddleInch = newMiddleInch;

    QAngle newAngle = ((newLeftInch - newRightInch) / m_chassisWidth) * radian;
    QAngle dAngle = newAngle - state.theta;

    // std::cout << "dA: " << dA << std::endl;

    QLength dAvgMainInch = (dLeftInch + dRightInch) / 2;

    QLength localOffX = 0_in, localOffY = 0_in;

    if(dAngle == 0.0)
    {
      localOffX = dMiddleInch;
      localOffY = dAvgMainInch;
    }
    else
    {
      localOffX = 2 * sin(dAngle.convert(radian) / 2) * ((dMiddleInch.convert(inch) / dAngle.convert(radian)) + m_distanceMiddle.convert(inch)) * inch;
      localOffY = 2 * sin(dAngle.convert(radian) / 2) * ((dAvgMainInch.convert(inch) / dAngle.convert(radian)) + (m_chassisWidth.convert(inch) / 2)) * inch;
    }

    QAngle avgAngle = state.theta + (dAngle / 2);

    QLength polarR = sqrt(std::pow(localOffX.convert(inch), 2) + std::pow(localOffY.convert(inch), 2)) * inch;
    QAngle polarA = (atan2(localOffY.convert(inch), localOffX.convert(inch)) * radian) - avgAngle;

    QLength dX = cos(polarA.convert(radian)) * polarR;
    QLength dY = sin(polarA.convert(radian)) * polarR;


    state.x += dX;
    state.y += dY;
    state.theta = newAngle;

  }

  void OdomTracker::setState(QLength x, QLength y, QAngle a)
  {
    state.x = x;
    state.y = y;
    state.theta = a;
  }

  void OdomTracker::setState(OdomState state)
  {
    state.x = state.x;
    state.y = state.y;
    state.theta = state.theta;
  }

  void OdomTracker::resetState()
  {
    state.x = 0_in;
    state.y = 0_in;
    state.theta = 0_rad;
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
