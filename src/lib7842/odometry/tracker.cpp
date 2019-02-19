#include "tracker.hpp"

namespace lib7842
{

  OdomTracker::OdomTracker
  (
    std::shared_ptr<okapi::SkidSteerModel> imodel,
    QLength chassisWidth, QLength wheelDiam, double ticksPerRotation,
    std::function<void(OdomTracker*)> trackerFunc
  ):
  model(imodel),
  m_chassisWidth(chassisWidth),
  m_mainDegToInch(wheelDiam.convert(inch) * 1_pi / ticksPerRotation),
  m_trackerFunc(trackerFunc),
  m_trackerTask(odometryTask, this)
  {
    resetSensors();
    resetState();
  };


  void OdomTracker::debug() {
    printf (
      "x: %1.2f, y: %1.2f, theta: %1.2f\n",
      state.x.convert(inch),
      state.y.convert(inch),
      state.theta.convert(degree)
    );
  }


  void OdomTracker::setState(qPoint newState)
  {
    state.x = newState.x;
    state.y = newState.y;
    state.theta = newState.theta;
  }

  qPoint& OdomTracker::getState() { return state; }

  void OdomTracker::setX(QLength newX) { state.x = newX; }
  void OdomTracker::setY(QLength newY) { state.y = newY; }
  void OdomTracker::setTheta(QAngle newTheta) { state.theta = newTheta; }

  QLength& OdomTracker::getX() { return state.x; }
  QLength& OdomTracker::getY() { return state.y; }
  QAngle& OdomTracker::getTheta() { return state.theta; }


  void OdomTracker::resetState() {
    state.x = 0_in;
    state.y = 0_in;
    state.theta = 0_rad;
  }

  void OdomTracker::resetSensors() {
    m_lastLeftInch = 0_in;
    m_lastRightInch = 0_in;
    model->resetSensors();
    m_lastLeftInch = 0_in;
    m_lastRightInch = 0_in;
  }


  void OdomTracker::odometryTask(void* input)
  {
    OdomTracker* that = static_cast<OdomTracker*>(input);
    std::cout << "Begin Tracking" << std::endl;

    int count = 0;
    while(true) {
      count++;
      if(count >= 40) { that->debug(); count = 0; }

      that->m_trackerFunc(that);
      pros::delay(10);
    }
  }


  void OdomTracker::aTracking(OdomTracker* that) {
    that->aTracking();
  }

  void OdomTracker::aTracking()
  {
    std::valarray<int32_t> newTicks = model->getSensorVals();

    QLength newLeftInch = (newTicks[0] * m_mainDegToInch) * inch;
    QLength newRightInch = (newTicks[1] * m_mainDegToInch) * inch;

    QLength dLeftInch = newLeftInch - m_lastLeftInch;
    QLength dRightInch = newRightInch - m_lastRightInch;

    m_lastLeftInch = newLeftInch;
    m_lastRightInch = newRightInch;

    QAngle newAngle = state.theta + (((dLeftInch - dRightInch) / m_chassisWidth) * radian);
    QAngle dAngle = newAngle - state.theta;

    QLength dAvgMainInch = (dLeftInch + dRightInch) / 2;

    QLength localOffX = 0_in, localOffY = 0_in;

    if(dAngle == 0.0_rad)
    {
      localOffY = dAvgMainInch;
    }
    else
    {
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

    state.theta = OdomMath::rollAngle180(state.theta);
  }


}
