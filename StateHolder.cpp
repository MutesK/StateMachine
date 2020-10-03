//
// Created by JunMin Kim on 2020/09/30.
//

#include <utility>
#include "StateMachine.hpp"
#include "StateHolder.hpp"

using WeakStatePtr = std::weak_ptr<IState>;

struct AssignStatePointer
{
    static void Do(WeakStatePtr& TargetPtr, const StatePtr& SourcePtr)
    {
        TargetPtr = std::make_shared<std::remove_reference<decltype(*std::declval<decltype(SourcePtr)>())>::type>(*SourcePtr);
    }
};

struct CheckTransitionAndDo
{
    static void Do(WeakStatePtr& Ptr, const TransitionGraph& Graph)
    {
        const auto& StateMap = Graph.StateMap;
        const auto& ConditionMap = Graph.ConditionMap;
        const auto& Conditions = Graph.StatePerConditionGraph;

        const auto& CanTransitionIter = std::find_if(Conditions.begin(), Conditions.end(),
                [&](const ConditionID id)
                {
                    const auto& ConditionPtr = ConditionMap[id];

                    if(ConditionPtr->IsTransition(Ptr))
                        return true;

                    return false;
                });

        if(CanTransitionIter == Conditions.end())
            return;

        const auto& TransiteTargetStateID = Graph.TransitionConditionPerNextState[CanTransitionIter->second];
        const auto& TransiteTargetStatePtr = StateMap[TransiteTargetStateID];

        Ptr->OnLeave();
        Ptr.reset();
        AssignStatePointer::Do(Ptr, TransiteTargetStatePtr);
        Ptr->OnEnter();
    }
};


StateHolder::StateHolder(const TransitionGraph& Graph)
    :_TransitionGraph(Graph)
{
    AssignStatePointer::Do(_CurrentState, Graph.GetInitialStatePtr());
}

void StateHolder::Tick()
{
    _CurerntState->Process();

    CheckTransitionAndDo::Do(_CurerntState, _TransitionGraph);
}
