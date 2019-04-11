#include "asyncAction.hpp"
#include "controller.hpp"

namespace lib7842
{

  AsyncAction::AsyncAction() {}


  AsyncAction &AsyncAction::withExclusion(AsyncActionRef action, exclusionTypes type) {
    m_exclusions.push_back({action, type});
    return *this;
  }

  AsyncAction &AsyncAction::withAction(actionFunction action, actionTypes type) {
    m_actions.push_back({action, type, false});
    return *this;
  }

  AsyncAction &AsyncAction::withTrigger(triggerFunction trigger, triggerTypes type) {
    m_triggers.push_back({trigger, type});
    return *this;
  }

  AsyncAction &AsyncAction::withTrigger(QPoint point, QLength distanceThresh, triggerTypes type) {
    m_triggers.push_back({[=](OdomController* that){ return that->distanceToPoint(mirrorSide(point, m_side)) < distanceThresh; }, type});
    return *this;
  }

  AsyncAction &AsyncAction::withTrigger(QPoint point, QLength distanceThresh, QAngle angleThresh, triggerTypes type) {
    m_triggers.push_back({[=](OdomController* that){ return that->distanceToPoint(mirrorSide(point, m_side)) < distanceThresh && that->angleToPoint(mirrorSide(point, m_side)).abs() < angleThresh; }, type});
    return *this;
  }

  AsyncAction &AsyncAction::withTrigger(QPoint point, QAngle angleThresh, triggerTypes type) {
    m_triggers.push_back({[=](OdomController* that){ return that->angleToPoint(mirrorSide(point, m_side)).abs() < angleThresh; }, type});
    return *this;
  }

  AsyncAction &AsyncAction::withTrigger(QAngle angle, QAngle angleThresh, triggerTypes type) {
    m_triggers.push_back({[=](OdomController* that){ return that->tracker->getTheta() > mirrorSide(angle, m_side) - angleThresh && that->tracker->getTheta() < mirrorSide(angle, m_side) + angleThresh; }, type});
    return *this;
  }

  AsyncAction &AsyncAction::withSide(autonSides side) {
    m_side = side;
    return *this;
  }



  void AsyncAction::run(OdomController* that)
  {

    /**
    * Exclusions
    */
    for(exclusionGroup &exclusion : m_exclusions)
    {
      switch(std::get<1>(exclusion))
      {
        case exclusionTypes::onlyBefore:
        {
          if(std::get<0>(exclusion).get().m_triggered) { return; }
          break;
        }
        case exclusionTypes::onlyAfter:
        {
          if(!std::get<0>(exclusion).get().m_triggered) { return; }
          break;
        }
      }
    }


    /**
    * Triggers
    * This goes through all the onlyBefores and Afters before checking the actual trigger
    */
    for(triggerGroup &trigger : m_triggers)
    {
      switch(std::get<1>(trigger))
      {
        case triggerTypes::onlyBefore:
        {
          if(std::get<0>(trigger)(that)) { return; }
          break;
        }
        case triggerTypes::onlyAfter:
        {
          if(!std::get<0>(trigger)(that)) { return; }
          break;
        }
        default: break;
      }
    }
    for(triggerGroup &trigger : m_triggers)
    {
      switch(std::get<1>(trigger))
      {
        case triggerTypes::trigger:
        {
          if(std::get<0>(trigger)(that) && !m_triggered) { m_triggered = true; }
          break;
        }
        default: break;
      }
    }

    /**
    * Actions
    */
    for(actionGroup &action : m_actions)
    {
      switch(std::get<1>(action))
      {
        case actionTypes::onceBefore:
        {
          if(!std::get<2>(action)) {
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
        case actionTypes::onceUnlessTriggered:
        {
          if(!std::get<2>(action) && !m_triggered) {
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
        case actionTypes::continousAfter:
        {
          if(m_triggered) { std::get<0>(action)(); }
          break;
        }
        case actionTypes::continous:
        {
          std::get<0>(action)();
          break;
        }
      }
    }

  }

}
