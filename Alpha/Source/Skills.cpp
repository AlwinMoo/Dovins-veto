#include "Skills.h"
#include "Rendering.hpp"
#include <iostream>


void skills_upgrade_check(GameObject* player)
{
	if ((player->Range.skill_bit & base) != base && AEInputCheckTriggered(AEVK_H))
	{
		player->Range.skill_bit = base;
		player->Range.first_tier.cooldown = 0.0f;
		player->Range.first_tier.damage = 1.0f;
		player->Range.timer = 0.0f;
		std::cout << "shoot active\n";
	}

	if ((player->Range.skill_bit & tier1) != tier1 && AEInputCheckTriggered(AEVK_B))
	{
		player->Range.skill_bit |= tier1;
		std::cout << "exploding active";
	}

	if((player->Range.skill_bit & tier2) != tier2 && AEInputCheckTriggered(AEVK_N))
	{
		player->Range.skill_bit |= tier2;
		player->Range.second_tier.cooldown = 0.0f;
		std::cout << "car active";
	}

	if ((player->Melee.skill_bit & base) != base && AEInputCheckTriggered(AEVK_J))
	{
		player->Melee.skill_bit |= base;
		player->Melee.first_tier.cooldown = 0.0f;
		player->Melee.first_tier.damage = 1.0f;
		player->Melee.timer = 0.0f;
		std::cout << "AOE active\n";
	}

	if ((player->Melee.skill_bit & tier1) != tier1 && AEInputCheckTriggered(AEVK_U))
	{
		player->Melee.skill_bit |= tier1;
		std::cout << "taunt active";
	}

	if ((player->Melee.skill_bit & tier2) != tier2 && AEInputCheckTriggered(AEVK_I))
	{
		player->Melee.skill_bit |= tier2;
		std::cout << "Blasting";
	}

	if ((player->Utility.skill_bit & base) != base && AEInputCheckTriggered(AEVK_O))
	{
		player->Utility.skill_bit |= base;
		std::cout << "blink active\n";
	}
}

void shoot_bullet(GameObject* Player, GameObject* skill_inst)
{
	double bullet_ang;
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	
	skill_inst->type = GameObject::GAMEOBJECT_TYPE::GO_BULLET;
	skill_inst->position.x = Player->position.x;
	skill_inst->position.y = Player->position.y;
	skill_inst->scale.x = skill_vals::BULLET_SIZE;
	skill_inst->scale.y = skill_vals::BULLET_SIZE;
	bullet_ang = atan2(static_cast<double> (skill_inst->position.y - mouseY), static_cast<double> (skill_inst->position.x - mouseX));

	AEVec2Set(&skill_inst->direction, -cos(bullet_ang), -sin(bullet_ang));
	AEVec2Normalize(&skill_inst->direction, &skill_inst->direction);
		switch (Player->Range.skill_bit)
		{
			case(base):
				skill_inst->Range.skill_bit = base;
				//can update damage numbers here
				break;
			case(tier1):
				skill_inst->Range.skill_bit = tier1;
				//here too
				break;
			case(tier2):
				skill_inst->Range.skill_bit = tier2;
				//here too
			default:
				break;
		}
	//	break;
	//}
}

void spreadshot(GameObject* parent, GameObject* skill_inst, int times)
{
	skill_inst->type = GameObject::GAMEOBJECT_TYPE::GO_BULLET;
	skill_inst->position.x = parent->position.x;
	skill_inst->position.y = parent->position.y;
	skill_inst->scale.x = skill_vals::BULLET_SIZE;
	skill_inst->scale.y = skill_vals::BULLET_SIZE;
	switch(times)
	{ 
	case 0 :
		AEVec2Set(&skill_inst->direction, cos(PI), sin(PI));
		break;
	case 1:
		AEVec2Set(&skill_inst->direction, -cos(PI), sin(PI));
		break;
	case 2:
		AEVec2Set(&skill_inst->direction, cos(0.5 * PI), sin(0.5 * PI));
		break;
	case 3:
		AEVec2Set(&skill_inst->direction, cos(0.5 * PI), -sin(0.5 * PI));
		break;
	default:
		break;
	}
}

void car_move(GameObject* Player, GameObject* skill_inst)
{
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	double car_ang;
	
	skill_inst->type = GameObject::GAMEOBJECT_TYPE::GO_CAR;
	skill_inst->position.x = Player->position.x;
	skill_inst->position.y = Player->position.y;
	skill_inst->scale.x = skill_vals::CAR_SIZE;
	skill_inst->scale.y = skill_vals::CAR_SIZE;
	skill_inst->alpha = 1.0f;
	car_ang = atan2(static_cast<double>(skill_inst->position.y - mouseY), static_cast<double> (skill_inst->position.x - mouseX));
	AEVec2Set(&skill_inst->direction, -cos(car_ang), -sin(car_ang));
}

void random_shoot(GameObject* parent, GameObject* skill_inst)
{
	skill_inst->type = GameObject::GO_BULLET;
	skill_inst->position.x = parent->position.x;
	skill_inst->position.y = parent->position.y;
	skill_inst->scale.x = skill_vals::BULLET_SIZE;
	skill_inst->scale.y = skill_vals::BULLET_SIZE;
	skill_inst->alpha = 1.0f;
	AEVec2Set(&skill_inst->direction, -cos(2 * PI * AERandFloat()), -sin(2 * PI * AERandFloat()));
}

void AOE_move(GameObject* Player, GameObject* skill_inst)
{
	skill_inst->type = GameObject::GO_AOE;
	skill_inst->position.x = Player->position.x;
	skill_inst->position.y = Player->position.y;
	skill_inst->scale.x = skill_vals::AOE_SIZE;
	skill_inst->scale.y = skill_vals::AOE_SIZE;
	skill_inst->alpha = skill_vals::AOE_ALPHA;
	AEVec2Set(&skill_inst->direction, 0, 0);

}

void AOE_ready(GameObject* player, GameObject* AOE)
{
	//tier 2 stuff
	if (player->Melee.timer > 0.2 && AOE->alpha < 1.0f && (player->Melee.skill_bit & tier2))
	{
		AOE->alpha += 0.1f;
		player->Melee.timer = 0;
	}

	if (AOE->alpha >= 1.0f && !AOE->skill_flag && (player->Melee.skill_bit & tier2))
	{
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);
		double AOE_ang;
		AOE_ang = atan2(static_cast<double>(AOE->position.y - mouseY), static_cast<double> (AOE->position.x - mouseX));
		AEVec2Set(&AOE->direction, -cos(AOE_ang), -sin(AOE_ang));
		AEVec2 norm_direc;
		AEVec2Normalize(&norm_direc, &AOE->direction);
		AEVec2Set(&AOE->direction, norm_direc.x, norm_direc.y);
		AOE->skill_flag = true;
	}
}

void taunt_move(GameObject* player, GameObject* enemy)
{
	enemy->smallTarget = player;
	enemy->alpha = 0.5f;
}

void player_blink(GameObject* player)
{
	s32 mousex, mousey;
	AEInputGetCursorPosition(&mousex, &mousey);
	AEVec2 Mouse{ mousex, mousey };

	if (AEVec2Distance(&player->position, &Mouse) >= skill_vals::BLINK_RANGE) return;

	player->position.x = mousex;
	player->position.y = mousey;
}


int skill_input_check(GameObject* player)
{
	if (player->Melee.first_tier.on_cd) //AOE cooldown
	{
		player->Melee.first_tier.cooldown += AEFrameRateControllerGetFrameTime();
		if (player->Melee.first_tier.cooldown >= 5.0)
		{
			player->Melee.first_tier.cooldown = 0.0;
			player->Melee.first_tier.on_cd = false;
		}
	}

	if (player->Range.second_tier.on_cd) //car cooldown
	{
		player->Range.second_tier.cooldown += AEFrameRateControllerGetFrameTime();
		if (player->Range.second_tier.cooldown >= 10.0)
		{
			player->Range.second_tier.cooldown = 0.0;
			player->Range.second_tier.on_cd = false;

		}
	}

	if (player->Range.first_tier.on_cd) //shoot cooldown
	{
		player->Range.first_tier.cooldown += AEFrameRateControllerGetFrameTime();
		if (player->Range.first_tier.cooldown >= 0.2)
		{
			player->Range.first_tier.cooldown = 0.0;
			player->Range.first_tier.on_cd = false;
		}
	}

	if (player->Utility.first_tier.on_cd)
	{
		player->Utility.first_tier.cooldown += AEFrameRateControllerGetFrameTime();

		if (player->Utility.first_tier.cooldown >= 3.0)
		{
			player->Utility.first_tier.cooldown = 0.0;
			player->Utility.first_tier.on_cd = false;
		}
	}

	if (AEInputCheckCurr(AEVK_LBUTTON) && (player->Range.skill_bit & base) == base && !player->Range.first_tier.on_cd)
	{
		player->Range.first_tier.on_cd = true;
		return shooting;
	}

	if (AEInputCheckTriggered(AEVK_X) && (player->Melee.skill_bit & base) == base && !player->Melee.first_tier.on_cd) //AOE
	{
		player->Melee.first_tier.active = true;
		return AOEing;
	}

	if (AEInputCheckTriggered(AEVK_C) && (player->Range.skill_bit & tier2) && !player->Range.second_tier.on_cd) //car
	{
		player->Range.second_tier.active = true;
		player->Range.second_tier.on_cd = true;
		return car;
	}

	if (AEInputCheckTriggered(AEVK_V) && (player->Melee.skill_bit & tier1))
	{
		return taunt;
	}

	if (AEInputCheckTriggered(AEVK_LSHIFT) && player->Utility.skill_bit & base)
	{
		player->Utility.first_tier.on_cd = true;
		return blink;
	}
	return -1;
}

void afterimage(GameObject* clone, GameObject* player)
{
	clone->type		  = GameObject::GAMEOBJECT_TYPE::GO_CLONE;
	clone->position.x = player->position.x;
	clone->position.y = player->position.y;
	clone->alpha	  = skill_vals::CLONE_ALPHA;
	clone->scale.x	  = player->scale.x;
	clone->scale.y	  = player->scale.y;
	clone->active	  = true;
	clone->timer	  = 0.0;
	clone->direction  = { 0,0 };
}


