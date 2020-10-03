//
// Created by JunMin Kim on 2020/09/30.
//

#include "StateMachine.hpp"

struct CheckTransitionAndDo
{
    static void Do(StatePtr& Ptr, const TransitionGraph& Graph)
    {
        const auto& StateMap = Graph.StateMap;
        const auto& ConditionMap = Graph.ConditionMap;
        const auto& Conditions = Graph.StatePerConditionGraph;

        const auto& CanTransitionIter = std::find_if(Conditions.begin(), Conditions.end(),
                [&](const ConditionID id)
                {
                    const auto& ConditionPtr = Conditions[id];

                    if(ConditionPtr->IsTransition(Ptr))
                        return true;

                    return false;
                });

        if(CanTransitionIter == Conditions.end())
            return;

        const auto& TransiteTargetStateID = TransitionConditionPerNextState[CanTransitionIter->second];
        const auto& TransiteTargetStatePtr = StateMap[TransiteTargetStateID];

        Ptr->OnLeave();
        Ptr.reset();
        Ptr = std::make_shared<std::remove_reference<decltype(*std::declval<TransiteTargetStatePtr>())>::type>(*TransiteTargetStatePtr);
        Ptr->OnEnter();
    }
};


StateHolder::StateHolder(const TransitionGraph &Graph)
    :_TransitionGraph(Graph)
{
    *_CurerntState = *Graph.GetInitialStatePtr();
}

void StateHolder::Tick()
{
    _CurerntState->Process();

    CheckTransitionAndDo::Do(_CurerntState, _TransitionGraph);
}
