#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"
#include "controller.hpp"

namespace lib7842
{
  class OdomController;
  class AsyncAction;

  using AsyncActions = std::reference_wrapper<AsyncAction>;

  typedef std::function<bool(OdomController*)> triggerFunc_t;
  typedef std::function<void()> actionFunc_t;

  enum class actionTypes
  {
    onceBefore,
    onceAfter,
    continousBefore,
    continousAfter
  };

  enum class exclusionTypes
  {
    onlyBefore,
    onlyAfter
  };

  using actionGroup = std::tuple<actionFunc_t, actionTypes, bool>;
  using onlyGroup = std::tuple<AsyncActions, exclusionTypes>;

  class AsyncAction
  {

  private:

    std::vector<onlyGroup> m_exclusions;

    std::vector<triggerFunc_t> m_triggers;
    std::vector<actionGroup> m_actions;

    bool m_triggered = false;

  public:

    AsyncAction();

    AsyncAction &withExclusion(AsyncAction&, exclusionTypes);

    AsyncAction &withTrigger(triggerFunc_t);
    AsyncAction &withTrigger(qPoint, QLength);
    AsyncAction &withTrigger(qPoint, QLength, QAngle);
    AsyncAction &withTrigger(qPoint, QAngle);
    AsyncAction &withTrigger(QAngle, QAngle);

    AsyncAction &withAction(actionFunc_t, actionTypes = actionTypes::onceAfter);

    #define makeTrigger(x) [&](OdomController* that){x}
    #define makeAction(x) [&](){x}
    #define withMakeAction(x) withAction(makeAction(x))

    void run();

  };
}
