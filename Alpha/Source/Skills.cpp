#include "Skills.h"

namespace
{

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
		Bullet->scale.x = 2 * BULLET_SIZE;
		Bullet->scale.y = 2 * BULLET_SIZE;
	}

	if (flag == upgrade2)
	{
		Bullet->scale.x = 3 * BULLET_SIZE;
		Bullet->scale.y = 3 * BULLET_SIZE;
	}
}



