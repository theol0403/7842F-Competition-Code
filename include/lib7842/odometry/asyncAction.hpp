#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"

namespace lib7842
{
  typedef std::function<bool(OdomController*)> triggerFunc_t;
  typedef std::function<void()> actionFunc_t;

  class AsyncAction
  {

  private:

    bool m_triggered = false;
    std::vector<triggerFunc_t> m_triggers;
    std::vector<actionFunc_t> m_actions;

    std::vector<actionFunc_t> m_continuousActionBefore;
    std::vector<actionFunc_t> m_continuousActionAfter;

    std::vector<std::reference_wrapper<AsyncAction>> m_onlyBefores;
    std::vector<std::reference_wrapper<AsyncAction>> m_onlyAfters;

  public:

    AsyncAction();

    AsyncAction &withTrigger(triggerFunc_t);
    AsyncAction &withTrigger(qPoint, QLength);
    AsyncAction &withTrigger(qPoint, QLength, QAngle);
    AsyncAction &withTrigger(qPoint, QAngle);
    AsyncAction &withTrigger(QAngle);

    AsyncAction &withAction(actionFunc_t);
    AsyncAction &withContinuousActionBefore(actionFunc_t);
    AsyncAction &withContinuousActionAfter(actionFunc_t);
    AsyncAction &onlyBefore(AsyncAction&);
    AsyncAction &onlyAfter(AsyncAction&);

    #define .makeTrigger(x) .withTrigger([&](){x})
    #define .makeAction(x) .withAction([&](){x})
    
  }
}
