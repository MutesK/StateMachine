#include <iostream>
#include "StateMachine.hpp"

class ActionState : public IState, RegisterStaticObject<ActionState>
{
public:
    ActionState()
    {
        SET_OBJECT(ActionState);
    }

    virtual void OnEnter() override
    {
        std::cout << "ActionState Enter\n";
    }

    virtual void Process() override
    {
        std::cout << "ActionState Process\n";
    }

    virtual void OnLeave() override
    {
        std::cout << "ActionState Leaved\n";
    }

};

struct ActionToDieTransition : public ITransitionCondition, RegisterStaticObject<ActionToDieTransition>
{
    ActionToDieTransition()
    {
        SET_OBJECT(ActionToDieTransition);
    }

    virtual bool IsTransition() override
    {
        return true;
    }
};



class DieState : public IState, RegisterStaticObject<DieState>
{
public:
    DieState()
    {
        SET_OBJECT(DieState);
    }

    virtual void OnEnter() override
    {
        std::cout << "DieState Enter\n";

    }

    virtual void Process() override
    {
        std::cout << "DieState Enter\n";

    }

    virtual void OnLeave() override
    {
        std::cout << "DieState Enter\n";

    }
};

int main()
{
    TransitionGraph Graph;
    Graph.RegisterStateType<ActionState>(ActionState());
    Graph.RegisterStateType<DieState>(DieState());
    Graph.RegisterConditionClassType<ActionToDieTransition>(ActionToDieTransition());

    Graph.SetInitialStateClassType<ActionState>();
    Graph.AttachTransitionGraph<ActionState, DieState, ActionToDieTransition>();


    StateHolder Holder(Graph);

    while(true)
    {
        Holder.Tick();
    }


    return 0;
}
