//
// Created by JunMin Kim on 2020/09/30.
//

#ifndef STATEMACHINE_STATEHOLDER_HPP
#define STATEMACHINE_STATEHOLDER_HPP

#include "StateMachine.hpp"

class StateHolder
{
    const TransitionGraph&  _TransitionGraph;
    std::weak_ptr<IState>   _CurrentState;
public:
    StateHolder(const TransitionGraph& Graph);
    void Tick();
};


#endif //STATEMACHINE_STATEHOLDER_HPP
