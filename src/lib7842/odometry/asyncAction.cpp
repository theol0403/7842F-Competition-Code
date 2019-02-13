#include "asyncAction.hpp"

namespace lib7842
{

  AsyncAction::AsyncAction() {}

  AsyncAction &AsyncAction::withTrigger(triggerFunc_t trigger) { m_triggers.push_back(trigger); }
  AsyncAction &AsyncAction::withTrigger(qPoint point, QLength distanceThresh) { m_triggers.push_back([&](OdomController*){ return that->computeDistanceToPoint(point) < distanceThresh }); }
  AsyncAction &AsyncAction::withTrigger(qPoint point, QLength distanceThresh, QAngle angleThresh) { m_triggers.push_back([&](OdomController*){ return that->computeDistanceToPoint(point) < distanceThresh && that->computeAngleToPoint(point).abs() < angleThresh; }); }
  AsyncAction &AsyncAction::withTrigger(qPoint point, QAngle angleThresh) { m_triggers.push_back([&](OdomController*){ return that->computeAngleToPoint(point).abs() < angleThresh; }); }
  AsyncAction &AsyncAction::withTrigger(QAngle angle, QAngle angleThresh) { m_triggers.push_back([&](OdomController*){ return that->tracker->getTheta() > angle - angleThresh && that->tracker->getTheta() < angle + angleThresh; }); }

  AsyncAction &AsyncAction::withAction(actionFunc_t action) { m_actions.push_back(action); }
  AsyncAction &AsyncAction::withContinuousActionBefore(actionFunc_t action) { m_continuousActionBefore.push_back(action); }
  AsyncAction &AsyncAction::withContinuousActionAfter(actionFunc_t action) { m_continuousActionAfter.push_back(action); }
  AsyncAction &AsyncAction::onlyBefore(AsyncAction& action) { m_onlyBefores.push_back(action); }
  AsyncAction &AsyncAction::onlyAfter(AsyncAction& action) { m_onlyAfters.push_back(action); }

}
