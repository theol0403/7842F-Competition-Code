#include "odomTracker.hpp"

namespace lib7842
{

  OdomTracker::OdomTracker
  (
    std::shared_ptr<okapi::ThreeEncoderSkidSteerModel> imodel,
    QLength chassisWidth, QLength distanceMiddle,
    QLength wheelDiam,
    double mainTicksPerRev, double middleTicksPerRev
  ):
  model(imodel),

  m_chassisWidth(chassisWidth),
  m_distanceMiddle(distanceMiddle),

  m_mainDegToInch(wheelDiam.convert(inch) * 1_pi / mainTicksPerRev),
  m_middleDegToInch(wheelDiam.convert(inch) * 1_pi / middleTicksPerRev)
  {};


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
    std::valarray<int32_t> newTicks = model->getSensorVals();

    QLength newLeftInch = (newTicks[0] * m_mainDegToInch) * inch;
    QLength newRightInch = (newTicks[1] * m_mainDegToInch) * inch;
    QLength newMiddleInch = (newTicks[2] * m_middleDegToInch) * inch;

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

    if(dAngle == 0.0_rad)
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

    // //Wrap theta
    // if(state.theta > 180_deg) { state.theta = state.theta - 360_deg; }
    // if(state.theta < -180_deg) { state.theta = state.theta + 360_deg; }

  }

  void OdomTracker::setState(QLength x, QLength y, QAngle a)
  {
    state.x = x;
    state.y = y;
    state.theta = a;
  }

  void OdomTracker::setState(qPoint newState)
  {
    state.x = newState.x;
    state.y = newState.y;
    state.theta = newState.theta;
  }

  void OdomTracker::setX(QLength newX)
  {
    state.x = newX;
  }

  void OdomTracker::setY(QLength newY)
  {
    state.y = newY;
  }

  void OdomTracker::setTheta(QAngle newTheta)
  {
    state.theta = newTheta;
  }

  void OdomTracker::resetState()
  {
    state.x = 0_in;
    state.y = 0_in;
    state.theta = 0_rad;
  }

  void OdomTracker::resetSensors()
  {
    m_lastLeftInch = 0_in;
    m_lastRightInch = 0_in;
    m_lastMiddleInch = 0_in;
    model->resetSensors();
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
      if(count >= 10)
      {
      //  thisOdomTracker->debug();
        count = 0;
      }

      pros::delay(10);
    }
  }


}
