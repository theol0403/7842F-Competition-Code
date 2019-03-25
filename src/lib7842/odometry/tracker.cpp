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
  m_wheelDiam(wheelDiam),
  m_degToInch(wheelDiam.convert(inch) * 1_pi / ticksPerRotation),
  m_trackerFunc(trackerFunc),
  m_task(taskFnc, this)
  {
    resetSensors();
    resetState();
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
    printf (
      "left: %d, right: %d\n",
      model->getSensorVals()[0],
      model->getSensorVals()[1]
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
    m_lastLeft = 0_in;
    m_lastRight = 0_in;
    model->resetSensors();
    m_lastLeft = 0_in;
    m_lastRight = 0_in;
  }


  void OdomTracker::taskFnc(void* input)
  {
    OdomTracker* that = static_cast<OdomTracker*>(input);
    pros::delay(500);
    that->resetState();
    that->resetSensors();
    that->resetState();
    //std::cout << "Begin Tracking" << std::endl;

    int count = 0;
    while(true) {
      count++;
      //if(count >= 50) { that->debug(); count = 0; }

      that->m_trackerFunc(that);
      pros::delay(4);
    }
  }



  void OdomTracker::aTracking(OdomTracker* that) {
    that->m_aTracking();
  }

  void OdomTracker::m_aTracking()
  {
    std::valarray<int32_t> newTicks = model->getSensorVals();

    QLength newLeftInch = (newTicks[0] * m_degToInch) * inch;
    QLength newRightInch = (newTicks[1] * m_degToInch) * inch;

    QLength dLeftInch = newLeftInch - m_lastLeft;
    QLength dRightInch = newRightInch - m_lastRight;

    m_lastLeft = newLeftInch;
    m_lastRight = newRightInch;

    QAngle dAngle = ((dLeftInch - dRightInch).convert(inch) / m_chassisWidth.convert(inch)) * radian;
    QAngle newAngle = state.theta + dAngle;

    QLength dAvgInch = (dLeftInch + dRightInch) / 2;

    QLength localOffX = 0_in, localOffY = 0_in;

    if(dAngle == 0.0_rad) {
      localOffY = dAvgInch;
    } else {
      localOffY = 2 * sin(dAngle.convert(radian) / 2) * ((dAvgInch.convert(inch) / dAngle.convert(radian)) + (m_chassisWidth.convert(inch) / 2)) * inch;
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



  void OdomTracker::mTracking(OdomTracker* that) {
    that->m_mTracking();
  }

  void OdomTracker::m_mTracking()
  {
    std::valarray<int32_t> newTicks = model->getSensorVals();

    QLength dX = 0.0_in;
    QLength dY = 0.0_in;
    QAngle dTheta = 0.0_rad;

    QLength newLeftInch = (newTicks[0] * m_degToInch) * inch;
    QLength newRightInch = (newTicks[1] * m_degToInch) * inch;

    QLength dLeftInch = newLeftInch - m_lastLeft;
    QLength dRightInch = newRightInch - m_lastRight;

    QLength dCenterArc = ((dLeftInch.convert(inch) + dRightInch.convert(inch)) / 2.0) * inch;

    dTheta = ((dLeftInch - dRightInch).convert(inch) / m_chassisWidth.convert(inch)) * radian;

    QLength radius = (dTheta == 0_rad) ? 0_in : (dCenterArc.convert(inch) / dTheta.convert(radian)) * inch;

    dX = dTheta == 0_rad ? 0_in : (radius - (radius.convert(inch) * cos(dTheta.convert(radian))) * inch);
    dY = dTheta == 0_rad ? dCenterArc : (radius.convert(inch) * sin(dTheta.convert(radian))) * inch;

    state.x = (dX.convert(inch) * cos(state.theta.convert(radian)) + dY.convert(inch) * sin(state.theta.convert(radian)) + state.x.convert(inch)) * inch;
    state.y = (dY.convert(inch) * cos(state.theta.convert(radian)) - dX.convert(inch) * sin(state.theta.convert(radian)) + state.y.convert(inch)) * inch;

    state.theta = dTheta + state.theta;

    m_lastLeft = newLeftInch;
    m_lastRight = newRightInch;
  }


  void OdomTracker::mdTracking(OdomTracker* that) {
    that->m_mdTracking();
  }

  void OdomTracker::m_mdTracking()
  {
    std::valarray<int32_t> newTicks = model->getSensorVals();

    double dX = 0.0;
    double dY = 0.0;
    double dTheta = 0.0;

    double lCurrEnc = newTicks[0] * m_degToInch;
    double rCurrEnc = newTicks[1] * m_degToInch;

    double lDEnc = lCurrEnc - m_lastLeft.convert(inch);
    double rDEnc = rCurrEnc - m_lastRight.convert(inch);

    double dCenterArc = (lDEnc + rDEnc) / 2.0;

    dTheta = (lDEnc - rDEnc) / m_chassisWidth.convert(inch);

    double radius = (dTheta == 0) ? 0 : dCenterArc / dTheta;
    dX = dTheta == 0 ? 0 : (radius - radius * cos(dTheta));
    dY = dTheta == 0 ? dCenterArc : radius * sin(dTheta);

    state.x = (dX * cos(state.theta.convert(radian)) + dY * sin(state.theta.convert(radian)) + state.x.convert(inch)) * inch;
    state.y = (dY * cos(state.theta.convert(radian)) - dX * sin(state.theta.convert(radian)) + state.y.convert(inch)) * inch;

    //state.theta = ((dTheta * 180.0 / PI) + state.theta.convert(degree)) * degree;
    state.theta = dTheta * radian + state.theta;

    m_lastLeft = lCurrEnc * inch;
    m_lastRight = rCurrEnc * inch;
  }

}
