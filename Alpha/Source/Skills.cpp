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

	//for (GameObject* pInst : go_list)
	//{
	//	if (pInst->active) continue;

	//	pInst->type = GameObject::GO_BULLET;
	//	pInst->tex = pTex;
	//	pInst->position.x = Player->position.x;
	//	pInst->position.y = Player->position.y;
	//	pInst->scale.x = BULLET_SIZE;
	//	pInst->scale.y = BULLET_SIZE;
	//	bullet_ang = atan2(static_cast<double> (pInst->position.y - mouseY), static_cast<double> (pInst->position.x - mouseX));
	//	AEVec2Set(&pInst->direction, -cos(bullet_ang), -sin(bullet_ang));
	//	pInst->alpha = 1.0f;
	//	pInst->active = true;
		
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
			default:
				break;
		}
	//	break;
	//}
}

void spreadshot(GameObject* parent, GameObject* skill_inst)
{
	skill_inst->position.x = parent->position.x;
	skill_inst->position.y = parent->position.y;
	skill_inst->scale.x = BULLET_SIZE;
	skill_inst->scale.y = BULLET_SIZE;
	AEVec2Set(&skill_inst->direction, cos(PI), sin(PI));
}

void car_move(GameObject* Player, GameObject* skill_inst)
{
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	double car_ang;
	
	skill_inst->position.x = Player->position.x;
	skill_inst->position.y = Player->position.y;
	skill_inst->scale.x = CAR_SIZE;
	skill_inst->scale.y = CAR_SIZE;
	skill_inst->alpha = 1.0f;
	car_ang = atan2(static_cast<double>(skill_inst->position.y - mouseY), static_cast<double> (skill_inst->position.x - mouseX));
	AEVec2Set(&skill_inst->direction, -cos(car_ang), -sin(car_ang));
	//for (GameObject* car : go_list)
	//{
	//	if (car->active) continue;

	//	car->type = GameObject::GO_CAR;
	//	car->tex = pTex;
	//	car->scale.x = CAR_SIZE;
	//	car->scale.y = CAR_SIZE;
	//	car->alpha = 1.0f;
	//	car->position.x = Player->position.x;
	//	car->position.y = Player->position.y;
	//	car_ang = atan2(static_cast<double> (car->position.y - mouseY), static_cast<double> (car->position.x - mouseX));
	//	AEVec2Set(&car->direction, -cos(car_ang), -sin(car_ang));
	//	car->active = true;
	//}
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
	//for (GameObject* pInst : go_list)
	//{
	//	if (pInst->active) continue;
	//	
	//	pInst->type = GameObject::GO_AOE;
	//	pInst->tex = pTex;
	//	pInst->position.x = Player->position.x;
	//	pInst->position.y = Player->position.y;
	//	pInst->scale.x = AOE_SIZE;
	//	pInst->scale.y = AOE_SIZE;
	//	pInst->alpha = AOE_ALPHA;
	//	AEVec2Set(&pInst->direction, 0.0f, 0.0f);
	//	pInst->active = true;
	//	break;
	//}
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
			player->AOE.cooldown = 0;
			player->AOE.on_cd = false;
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

	if (AEInputCheckTriggered(AEVK_C) && (player->Range.skill_bit & tier2))
	{
		return 2;
	}

	return -1;
}



