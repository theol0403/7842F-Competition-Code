#pragma once
#include "main.h"

namespace lib7842
{
  class AsyncAction
  {

    typedef std::function<bool(OdomController*)> triggerFunc_t;
    typedef std::function<void(OdomController*)> actionFunc_t;

  private:

    bool m_triggered = false;
    triggerFunc_t m_trigger;
    actionFunc_t m_action;

    std::vector<actionFunc_t> m_continuousActionsBefore;
    std::vector<actionFunc_t> m_continuousActionsAfter;

    std::vector<AsyncAction*> m_onlyBefores;
    std::vector<AsyncAction*> m_onlyAfters;

  public:

    AsyncAction();

    AsyncAction &withTrigger(triggerFunc_t);
    AsyncAction &withAction(actionFunc_t);
    AsyncAction &withContinuousActionBefore(actionFunc_t);
    AsyncAction &withContinuousActionAfter(actionFunc_t);
    AsyncAction &onlyBefore(AsyncAction&);
    AsyncAction &onlyAfter(AsyncAction&);


    // triggerFunc_t makeTrigger(qPoint, QLength);
    // triggerFunc_t makeTrigger(qPoint, QLength, QAngle);
    // triggerFunc_t makeTrigger(qPoint, QAngle);
    // triggerFunc_t makeTrigger(QAngle);
    // #define makeAction(x) [=](OdomController* that){x}
