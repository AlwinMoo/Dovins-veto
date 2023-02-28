#include "Skills.h"

namespace
{
	//bullet stuff
	const float BULLET_SIZE{ 10.0f };
	const float BULLET_VEL{ 20.0f };

	//AOE stuff
	const float AOE_SIZE{ 200.0f };
	const float AOE_ALPHA{ 0.5f };
}
void shoot_bullet(GameObject* Bullet, GameObject* Player, f32 mouseX, f32 mouseY, int flag)
{
	double bullet_ang;
	Bullet->position.x = Player->position.x;
	Bullet->position.y = Player->position.y;
	bullet_ang = atan2(static_cast<double>(Bullet->position.y - (mouseY)), static_cast<double> (Bullet->position.x - (mouseX)));
	Bullet->active = true;

	AEVec2Set(&Bullet->direction, -cos(bullet_ang), -sin(bullet_ang));
	if (flag == no_upgrades)
	{
		Bullet->scale.x = BULLET_SIZE;
		Bullet->scale.y = BULLET_SIZE;
	}

	if (flag == upgrade1)
	{
		Bullet->scale.x = 1.5 * BULLET_SIZE;
		Bullet->scale.y = 1.5 * BULLET_SIZE;
	}

	if (flag == upgrade2)
	{
		Bullet->scale.x = 1.5 * BULLET_SIZE;
		Bullet->scale.y = 1.5 * BULLET_SIZE;
	}
}

void AOE_move(GameObject* AOE, double player_x, double player_y)
{
	AOE->position.x = player_x;
	AOE->position.y = player_y;
	AOE->scale.x = AOE_SIZE;
	AOE->scale.y = AOE_SIZE;
	AOE->alpha = AOE_ALPHA;

}



