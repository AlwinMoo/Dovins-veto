#include "StateMachine.h"

void StateContext::set_state(PF_STATE state)
{
	currentState = state;
}

void StateContext::run_state()
{
	currentState();
}


