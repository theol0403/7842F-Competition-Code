#include "asyncAction.hpp"

namespace lib7842
{

  AsyncAction::AsyncAction() {}

  AsyncAction &AsyncAction::withTrigger(triggerFunc_t) { m_triggers.push_back(triggerFunc_t); }
  AsyncAction &AsyncAction::withTrigger(qPoint point, QLength distanceThresh) { m_triggers.push_back([&](){ return computeDistanceToPoint(point) < distanceThresh }); }
  AsyncAction &AsyncAction::withTrigger(qPoint point, QLength distanceThresh, QAngle angleThresh) { m_triggers.push_back([&](){ return computeDistanceToPoint(point) < distanceThresh && computeAngleToPoint(point).abs() < angleThresh; }); }
  AsyncAction &AsyncAction::withTrigger(qPoint point, QAngle angleThresh) { m_triggers.push_back([&](){ return computeAngleToPoint(point).abs() < angleThresh; }); }
  AsyncAction &AsyncAction::withTrigger(QAngle angle, QAngle angleThresh) { m_triggers.push_back([&](){ return tracker->getTheta() > angle - angleThresh && tracker->getTheta() < angle + angleThresh; }); }

  AsyncAction &AsyncAction::withAction(actionFunc_t);
  AsyncAction &AsyncAction::withContinuousActionBefore(actionFunc_t);
  AsyncAction &AsyncAction::withContinuousActionAfter(actionFunc_t);
  AsyncAction &AsyncAction::onlyBefore(AsyncAction&);
  AsyncAction &AsyncAction::onlyAfter(AsyncAction&);
