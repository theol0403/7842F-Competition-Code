#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"

namespace lib7842
{
  class OdomController;
  class AsyncAction;

  using AsyncActionRef = std::reference_wrapper<AsyncAction>;
  using AsyncActionList = std::vector<AsyncActionRef>;

  typedef std::function<bool(OdomController*)> triggerFunction;
  typedef std::function<void()> actionFunction;

  enum class exclusionTypes {
    onlyBefore,
    onlyAfter
  };

  enum class triggerTypes {
    trigger,
    onlyBefore,
    onlyAfter
  };

  enum class actionTypes {
    onceBefore,
    onceAfter,
    onceUnlessTriggered,
    continousBefore,
    continousAfter,
    continous
  };

  class AsyncAction
  {

  private:

    using exclusionGroup = std::tuple<AsyncActionRef, exclusionTypes>;
    std::vector<exclusionGroup> m_exclusions;

    using triggerGroup = std::tuple<triggerFunction, triggerTypes>;
    std::vector<triggerGroup> m_triggers;

    using actionGroup = std::tuple<actionFunction, actionTypes, bool>;
    std::vector<actionGroup> m_actions;

    bool m_triggered = false;

  public:

    AsyncAction();

    AsyncAction &withExclusion(AsyncActionRef, exclusionTypes);

    AsyncAction &withAction(actionFunction, actionTypes = actionTypes::onceAfter);

    AsyncAction &withTrigger(triggerFunction, triggerTypes = triggerTypes::trigger);
    AsyncAction &withTrigger(qPoint, QLength, triggerTypes = triggerTypes::trigger);
    AsyncAction &withTrigger(qPoint, QLength, QAngle, triggerTypes = triggerTypes::trigger);
    AsyncAction &withTrigger(qPoint, QAngle, triggerTypes = triggerTypes::trigger);
    AsyncAction &withTrigger(QAngle, QAngle, triggerTypes = triggerTypes::trigger);

    #define makeTrigger(x) [&](OdomController* that){x}
    #define makeAction(x) [&](){x}
    #define withMakeAction(x) withAction(makeAction(x))

    void run(OdomController* = nullptr);

  };
}
