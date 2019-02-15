#pragma once
#include "main.h"
#include "odomMath.hpp"
#include "tracker.hpp"
#include "controller.hpp"

namespace lib7842
{
  class OdomController;
  class AsyncAction;

  using AsyncActionRef = std::reference_wrapper<AsyncAction>;

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

    using exclusionGroup = std::tuple<AsyncActionRef, exclusionTypes>;
    std::vector<exclusionGroup> m_exclusions;

    using actionGroup = std::tuple<actionFunction, actionTypes, bool>;
    std::vector<actionGroup> m_actions;

    std::vector<triggerFunction> m_triggers;

    bool m_triggered = false;

  public:

    AsyncAction();

    AsyncAction &withExclusion(AsyncActionRef, exclusionTypes);

    AsyncAction &withAction(actionFunction, actionTypes = actionTypes::onceAfter);

    AsyncAction &withTrigger(triggerFunction);
    AsyncAction &withTrigger(qPoint, QLength);
    AsyncAction &withTrigger(qPoint, QLength, QAngle);
    AsyncAction &withTrigger(qPoint, QAngle);
    AsyncAction &withTrigger(QAngle, QAngle);

    #define makeTrigger(x) [&](OdomController* that){x}
    #define makeAction(x) [&](){x}
    #define withMakeAction(x) withAction(makeAction(x))

    void run(OdomController*);

  };
}
