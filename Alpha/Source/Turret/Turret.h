#pragma once
#include "../Enemy/StateMachine.h"

#include <iostream>

class Turret
{
public:
	Turret();
	~Turret();

	StateContext State;

	//States
	enum class tStates
	{
		TUR_IDLE,
		TUR_SHOOT,

		NUM_TUR_STATES
	};

	void Update();
};
