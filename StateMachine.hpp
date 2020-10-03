//
// Created by JunMin Kim on 2020/09/30.
//

#ifndef STATEMACHINE_STATEMACHINE_HPP
#define STATEMACHINE_STATEMACHINE_HPP

#include <vector>
#include <unordered_map>
#include <set>
#include <memory>
#include <functional>
#include <type_traits>
#include "StaticObject.hpp"


struct ITransitionCondition
{
    ITransitionCondition() = default;

    virtual bool IsTransition() = 0;
};

class IState
{
protected:
    IState() = default;

    virtual void OnEnter() = 0;
    virtual void Process() = 0;
    virtual void OnLeave() = 0;
};

using StateID = uint64_t;
using ConditionID = uint64_t;
using ConditionSet = std::set<ConditionID>;

using StatePtr = std::shared_ptr<IState>;
using ConditionPtr = std::shared_ptr<ITransitionCondition>;

class TransitionGraph
{
    friend struct CheckTransitionAndDo;

    StateID                               InitStateID = -1;

    std::unordered_map<StateID, StatePtr>   StateMap;
    std::unordered_map<ConditionID, ConditionPtr> ConditionMap;

    std::unordered_map<StateID, ConditionSet> StatePerConditionGraph;
    std::unordered_map<ConditionID , StateID> TransitionConditionPerNextState;
public:
    template <class StateClassType>
            void RegisterStateType(const StateClassType& CreatedClassObject);

    template <class ConditionClassType>
            void RegisterConditionClassType(const ConditionClassType& CreatedClassObject);

    template <class StateType, class MovedStateType, class TransitionConditionType>
            void AttachTransitionGraph();

    template <class StateClassType>
            void SetInitialStateClassType();

    StatePtr    GetInitialStatePtr() const;

private:
    StateID     GetInitialStateClassID() const;
};

template<class StateClassType>
void TransitionGraph::RegisterStateType(const StateClassType& CreatedClassObject)
{
    static_assert(std::is_base_of_v<IState, StateClassType>, "");
    static_assert(std::is_base_of_v<RegisterStaticObject<StateClassType>, StateClassType>, "");

    static_assert(std::is_copy_constructible_v<StateClassType>, "");

    StatePtr TempPtr = std::make_shared<StateClassType>(CreatedClassObject);
    StateMap[RegisterStaticObject<StateClassType>::StaticClassID()] = TempPtr;
}

template<class ConditionClassType>
void TransitionGraph::RegisterConditionClassType(const ConditionClassType& CreatedClassObject)
{
    static_assert(std::is_base_of_v<ITransitionCondition, ConditionClassType>, "");
    static_assert(std::is_base_of_v<RegisterStaticObject<ConditionClassType>, ConditionClassType>, "");

    static_assert(std::is_copy_constructible_v<ConditionClassType>, "");

    ConditionPtr TempPtr = std::make_shared<ConditionClassType>(CreatedClassObject);
    ConditionMap[RegisterStaticObject<ConditionClassType>::StaticClassID()] = TempPtr;
}

template<class StateType, class MovedStateType, class TransitionConditionType>
void TransitionGraph::AttachTransitionGraph()
{
    static_assert(std::is_base_of_v<IState, StateType>, "");
    static_assert(std::is_base_of_v<IState, MovedStateType>, "");
    static_assert(std::is_base_of_v<ITransitionCondition, TransitionConditionType>, "");

    StatePerConditionGraph[RegisterStaticObject<StateType>::StaticClassID()].insert(RegisterStaticObject<TransitionConditionType>::StaticClassID());
    TransitionConditionPerNextState[RegisterStaticObject<TransitionConditionType>::StaticClassID()] = RegisterStaticObject<MovedStateType>::StaticClassID();
}

template<class StateClassType>
void TransitionGraph::SetInitialStateClassType()
{
    static_assert(std::is_base_of_v<IState, StateClassType>, "");

    if(StateMap.end() == StateMap.find(RegisterStaticObject<StateClassType>::StaticClassID()))
        return;

    InitStateID = RegisterStaticObject<StateClassType>::StaticClassID();
}

StateID TransitionGraph::GetInitialStateClassID() const
{
    return InitStateID;
}

StatePtr TransitionGraph::GetInitialStatePtr() const
{
    const auto& Iter = StateMap.find(GetInitialStateClassID());

    if(Iter == StateMap.end())
        return nullptr;

    return Iter->second;
}


#endif //STATEMACHINE_STATEMACHINE_HPP
