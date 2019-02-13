#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"
#include "controller.hpp"

namespace lib7842
{
  class OdomController;
  class AsyncAction;

  using AsyncActionList = std::vector<std::reference_wrapper<AsyncAction>>;

  typedef std::function<bool(OdomController*)> triggerFunc_t;
  typedef std::function<void()> actionFunc_t;

  enum class actionTypes
  {
    onceBefore,
    onceAfter,
    continousBefore,
    continousAfter
  };

  struct actionAndType
  {
    actionFunc_t action;
    actionTypes type;
  };


  class AsyncAction
  {

  private:

    std::vector<triggerFunc_t> m_triggers;
    std::vector<actionAndType> m_actions;

    std::vector<std::reference_wrapper<AsyncAction>> m_onlyBefores;
    std::vector<std::reference_wrapper<AsyncAction>> m_onlyAfters;

  public:

    AsyncAction();

    AsyncAction &withTrigger(triggerFunc_t);
    AsyncAction &withTrigger(qPoint, QLength);
    AsyncAction &withTrigger(qPoint, QLength, QAngle);
    AsyncAction &withTrigger(qPoint, QAngle);
    AsyncAction &withTrigger(QAngle, QAngle);

    AsyncAction &withAction(actionFunc_t, actionTypes = actionTypes::onceAfter);
    AsyncAction &onlyBefore(AsyncAction&);
    AsyncAction &onlyAfter(AsyncAction&);

    #define makeTrigger(x) [&](OdomController* that){x}
    #define makeAction(x) [&](){x}
    #define withMakeAction(x) withAction(makeAction(x))

  };
}
