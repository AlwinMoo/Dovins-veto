#include "Turret.h"

namespace
{
	void Shoot()
	{
		std::cout << "Bang!" << std::endl;
	}

	void Idle()
	{
		std::cout << "Idle" << std::endl;
	}
}

Turret::Turret()
{
	State.set_state(Idle);
}

Turret::~Turret()
{

}

void Turret::Update()
{
	State.run_state();
}
