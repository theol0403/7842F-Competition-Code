#include "asyncAction.hpp"

namespace lib7842
{

  AsyncAction::AsyncAction() {}


  AsyncAction &AsyncAction::withExclusion(AsyncActionRef action, exclusionTypes type) {
    m_exclusions.push_back({action, type});
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

    /**
    * Exclusions
    */
    for(exclusionGroup exclusion : m_excusions)
    {
      switch(std::get<1>(exclusion))
      {
        case exclusionTypes::onlyBefore:
        {
          if(std::get<0>(exclusion).m_triggered) { return; }
          break;
        }
        case exclusionTypes::onlyAfter:
        {
          if(!std::get<0>(exclusion).m_triggered) { return; }
          break;
        }
      }
    }

    /**
    * Actions
    */
    for(actionGroup action : m_actions)
    {
      switch(std::get<1>(action))
      {
        case actionTypes::onceBefore:
        {
          if(!std::get<2>(action) && !m_triggered) {
            std::get<2>(action) = true;
            std::get<0>(action)();
          }
          break;
        }
        case actionTypes::onceAfter:
        {
          if(!std::get<2>(action) && m_triggered) {
            std::get<2>(action) = true;
            std::get<0>(action)();
          }
          break;
        }
        case actionTypes::continousBefore:
        {
          if(!m_triggered) { std::get<0>(action)(); }
          break;
        }
        case actionTypes::continousBefore:
        {
          if(m_triggered) { std::get<0>(action)(); }
          break;
        }
      }
    }


    /**
    * Triggers
    */
    for(triggerFunction &trigger : m_triggers)
    {
      if(trigger(that) && !m_triggered) { m_triggered = true; }
    }

  }



}
