#pragma once

//class State
//{
//	virtual void behaviour() = 0;
//};

class StateContext
{
private:
	using PF_STATE = void(*)();
	PF_STATE currentState;

public:
	/*StateContext();
	~StateContext();*/

	void set_state(PF_STATE state);
	void run_state();
};
