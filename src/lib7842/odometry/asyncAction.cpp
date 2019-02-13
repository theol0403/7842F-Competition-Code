#include "asyncAction.hpp"

namespace lib7842
{

  AsyncAction::AsyncAction() {}

  AsyncAction &AsyncAction::withTrigger(triggerFunc_t trigger) { m_triggers.push_back(trigger); }
  AsyncAction &AsyncAction::withTrigger(qPoint point, QLength distanceThresh) { m_triggers.push_back([&](OdomController*){ return computeDistanceToPoint(point) < distanceThresh }); }
  AsyncAction &AsyncAction::withTrigger(qPoint point, QLength distanceThresh, QAngle angleThresh) { m_triggers.push_back([&](OdomController*){ return computeDistanceToPoint(point) < distanceThresh && computeAngleToPoint(point).abs() < angleThresh; }); }
  AsyncAction &AsyncAction::withTrigger(qPoint point, QAngle angleThresh) { m_triggers.push_back([&](OdomController*){ return computeAngleToPoint(point).abs() < angleThresh; }); }
  AsyncAction &AsyncAction::withTrigger(QAngle angle, QAngle angleThresh) { m_triggers.push_back([&](OdomController*){ return tracker->getTheta() > angle - angleThresh && tracker->getTheta() < angle + angleThresh; }); }

  AsyncAction &AsyncAction::withAction(actionFunc_t action) { m_actions.push_back(action); }
  AsyncAction &AsyncAction::withContinuousActionBefore(actionFunc_t action) { m_continuousActionBefore.push_back(action); }
  AsyncAction &AsyncAction::withContinuousActionAfter(actionFunc_t action) { m_continuousActionAfter.push_back(action); }
  AsyncAction &AsyncAction::onlyBefore(AsyncAction& action) { onlyBefores.push_back(action); }
  AsyncAction &AsyncAction::onlyAfter(AsyncAction& action) { onlyAfters.push_back(action); }

}
