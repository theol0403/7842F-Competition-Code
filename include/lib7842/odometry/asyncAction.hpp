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

  typedef std::function<bool(OdomController*)> triggerFunction;
  typedef std::function<void()> actionFunction;

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

  class AsyncAction
  {

  private:

    using actionGroup = std::tuple<actionFunction, actionTypes, bool>;
    using onlyGroup = std::tuple<AsyncActions, exclusionTypes>;

    std::vector<onlyGroup> m_exclusions;

    std::vector<triggerFunction> m_triggers;
    std::vector<actionGroup> m_actions;

    bool m_triggered = false;

  public:

    AsyncAction();

    AsyncAction &withExclusion(AsyncAction&, exclusionTypes);

    AsyncAction &withTrigger(triggerFunction);
    AsyncAction &withTrigger(qPoint, QLength);
    AsyncAction &withTrigger(qPoint, QLength, QAngle);
    AsyncAction &withTrigger(qPoint, QAngle);
    AsyncAction &withTrigger(QAngle, QAngle);

    AsyncAction &withAction(actionFunction, actionTypes = actionTypes::onceAfter);

    #define makeTrigger(x) [&](OdomController* that){x}
    #define makeAction(x) [&](){x}
    #define withMakeAction(x) withAction(makeAction(x))

    void run(OdomController*);

  };
}
