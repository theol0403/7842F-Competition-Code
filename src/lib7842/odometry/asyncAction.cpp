#include "asyncAction.hpp"

namespace lib7842
{

  AsyncAction::AsyncAction() {}


  AsyncAction &AsyncAction::onlyBefore(AsyncAction& action) {
    m_onlyBefores.push_back(action);
    return *this;
  }

  AsyncAction &AsyncAction::onlyAfter(AsyncAction& action) {
    m_onlyAfters.push_back(action);
    return *this;
  }


  AsyncAction &AsyncAction::withTrigger(triggerFunction trigger) {
    m_triggers.push_back(trigger);
    return *this;
  }

  AsyncAction &AsyncAction::withTrigger(qPoint point, QLength distanceThresh) {
    m_triggers.push_back([&](OdomController* that){ return that->computeDistanceToPoint(point) < distanceThresh; });
    return *this;
  }

  AsyncAction &AsyncAction::withTrigger(qPoint point, QLength distanceThresh, QAngle angleThresh) {
    m_triggers.push_back([&](OdomController* that){ return that->computeDistanceToPoint(point) < distanceThresh && that->computeAngleToPoint(point).abs() < angleThresh; });
    return *this;
  }

  AsyncAction &AsyncAction::withTrigger(qPoint point, QAngle angleThresh) {
    m_triggers.push_back([&](OdomController* that){ return that->computeAngleToPoint(point).abs() < angleThresh; });
    return *this;
  }

  AsyncAction &AsyncAction::withTrigger(QAngle angle, QAngle angleThresh) {
    m_triggers.push_back([&](OdomController* that){ return that->tracker->getTheta() > angle - angleThresh && that->tracker->getTheta() < angle + angleThresh; });
    return *this;
  }


  AsyncAction &AsyncAction::withAction(actionFunction action, actionTypes type) {
    m_actions.push_back({action, type});
    return *this;
  }



  void AsyncAction::run(OdomController* that)
  {

    for(AsyncAction &onlyBefore : m_onlyBefores)
    {
      if(onlyBefore.m_triggered)
      {
        return;
      }
    }

    for(AsyncAction &onlyAfter : m_onlyAfters)
    {
      if(!onlyAfter.m_triggered)
      {
        return;
      }
    }


    if(m_triggered) //if already triggered
    {
      //run continousAfter actions
    }
    else
    {
      //run continousBefore actions
    }


    bool anyTriggered = false;

    for(triggerFunction &trigger : m_triggers)
    {
      if(trigger(that))
      {
        anyTriggered = true;
      }
    }

    if(anyTriggered && !m_triggered)
    {
      //run once after triggers
      m_triggered = true;
    }


  }


}
