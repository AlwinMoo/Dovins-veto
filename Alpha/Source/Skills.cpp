#include "Skills.h"
#include "Rendering.hpp"
#include <iostream>
namespace
{
	//bullet stuff
	const float		BULLET_SIZE	{ 10.0f };
	const float		BULLET_VEL	{ 20.0f };
	const int		MAX_SPREAD	{ 5 };

	//car stuff
	const float		CAR_SIZE	{ 40.0f };

	//AOE stuff
	const float AOE_SIZE{ 200.0f };
	const float AOE_ALPHA{ 0.5f };

	//overall skills stuff
	const u32 blink_flag = 0b0000'0001;
	const u32 shoot_flag = 0b0000'0010;
	const u32 shoot_flag2 = 0b0000'0001;
	const u32 AOE_flag = 0b0000'0100;

	const int num_skills{ 3 }; // AOE, shoot and car
	skill_func skills_array[num_skills]{shoot_bullet, AOE_move};
}

void skills_upgrade_check(GameObject* player)
{
	if ((player->skill_flag & shoot_flag) != shoot_flag && AEInputCheckTriggered(AEVK_H))
	{
		player->skill_flag |= shoot_flag;
		player->Range.cooldown = 0.0f;
		player->Range.damage = 1.0f;
		player->Range.skill_bit = base;
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
		player->Range.cooldown = 0.0f;
		std::cout << "car active";
	}

	if ((player->skill_flag & AOE_flag) != AOE_flag && AEInputCheckTriggered(AEVK_J))
	{
		player->skill_flag |= AOE_flag;
		player->AOE.cooldown = 0.0f;
		player->AOE.damage = 1.0f;
		player->AOE.skill_bit = 0000'0000;
		player->Range.timer = 0.0f;
		std::cout << "AOE active\n";
	}

	if ((player->skill_flag & blink_flag) != blink_flag && AEInputCheckTriggered(AEVK_K))
	{
		player->skill_flag |= blink_flag;
		std::cout << "blink active\n";
	}
}

void shoot_bullet(GameObject* Player, GameObject* skill_inst)
{
	double bullet_ang;
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
		
	skill_inst->position.x = Player->position.x;
	skill_inst->position.y = Player->position.y;
	skill_inst->scale.x = BULLET_SIZE;
	skill_inst->scale.y = BULLET_SIZE;
	bullet_ang = atan2(static_cast<double> (skill_inst->position.y - mouseY), static_cast<double> (skill_inst->position.x - mouseX));
	AEVec2Set(&skill_inst->direction, -cos(bullet_ang), -sin(bullet_ang));
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
	skill_inst->scale.x = BULLET_SIZE;
	skill_inst->scale.y = BULLET_SIZE;
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
	skill_inst->scale.x = CAR_SIZE;
	skill_inst->scale.y = CAR_SIZE;
	skill_inst->alpha = 1.0f;
	car_ang = atan2(static_cast<double>(skill_inst->position.y - mouseY), static_cast<double> (skill_inst->position.x - mouseX));
	AEVec2Set(&skill_inst->direction, -cos(car_ang), -sin(car_ang));
}

void random_shoot(GameObject* parent, GameObject* skill_inst)
{
	skill_inst->type = GameObject::GO_BULLET;
	skill_inst->position.x = parent->position.x;
	skill_inst->position.y = parent->position.y;
	skill_inst->scale.x = BULLET_SIZE;
	skill_inst->scale.y = BULLET_SIZE;
	skill_inst->alpha = 1.0f;
	AEVec2Set(&skill_inst->direction, -cos(2 * PI * AERandFloat()), -sin(2 * PI * AERandFloat()));
}

void AOE_move(GameObject* Player, GameObject* skill_inst)
{
	skill_inst->type = GameObject::GO_AOE;
	skill_inst->position.x = Player->position.x;
	skill_inst->position.y = Player->position.y;
	skill_inst->scale.x = AOE_SIZE;
	skill_inst->scale.y = AOE_SIZE;
	skill_inst->alpha = AOE_ALPHA;
	AEVec2Set(&skill_inst->direction, 0, 0);
}

void player_blink(GameObject* player, f32 mousex, f32 mousey)
{
	player->position.x = mousex;
	player->position.y = mousey;
}


int skill_input_check(GameObject* player)
{
	if (player->AOE.on_cd) //AOE cooldown
	{
		player->AOE.cooldown += AEFrameRateControllerGetFrameTime();
		if (player->AOE.cooldown >= static_cast<f64> (5.0f))
		{
			player->AOE.cooldown = 0.f;
			player->AOE.on_cd = false;
		}
	}

	if (player->Range.on_cd) //car cooldown
	{
		player->Range.cooldown += AEFrameRateControllerGetFrameTime();
		if (player->Range.cooldown >= static_cast<f64> (10.0f))
		{
			player->Range.cooldown = 0.f;
			player->Range.on_cd = false;

		}
	}

	if (AEInputCheckTriggered(AEVK_Z) && (player->skill_flag & shoot_flag) == shoot_flag)
	{
		return 0;
	}

	if (AEInputCheckTriggered(AEVK_X) && (player->skill_flag & AOE_flag) == AOE_flag && !player->AOE.on_cd) 
	{
		player->AOE.on_cd = true;
		return 1;
	}

	if (AEInputCheckTriggered(AEVK_C) && (player->Range.skill_bit & tier2) && !player->Range.on_cd)
	{
		player->Range.active = true;
		player->Range.on_cd = true;
		return 2;
	}

	return -1;
}



