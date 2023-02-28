#pragma once

//class State
//{
//	virtual void behaviour() = 0;
//};

class StateContext
{
	using PF_STATE = void(*)();
	PF_STATE currentState;

public:
	void set_state(PF_STATE state);
	void run_state();
};
