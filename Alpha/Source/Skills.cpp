#include "Skills.h"
#include "Rendering.hpp"
#include <iostream>
namespace
{
	//bullet stuff
	const float BULLET_SIZE{ 10.0f };
	const float BULLET_VEL{ 20.0f };

	//AOE stuff
	const float AOE_SIZE{ 200.0f };
	const float AOE_ALPHA{ 0.5f };

	//overall skills stuff
	//u32 flag = 0b0000'0000;
	const u32 blink_flag = 0b0000'0001;
	const u32 shoot_flag = 0b0000'0010;
	const u32 AOE_flag = 0b0000'0100;

	const int num_skills{ 2 }; // AOE, shoot and blink
	skill_func skills_array[num_skills]{shoot_bullet, AOE_move};
}

void skills_upgrade_check(GameObject* player)
{
	if ((player->skill_flag & shoot_flag) != shoot_flag && AEInputCheckTriggered(AEVK_H))
	{
		player->skill_flag |= shoot_flag;
		player->Range.cooldown = 0.0f;
		player->Range.damage = 1.0f;
		player->Range.skill_bit = 0000'0000;
		player->Range.timer = 0.0f;
		std::cout << "shoot active\n";
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

void shoot_bullet(GameObject* Player, std::vector<GameObject*>& go_list, AEGfxTexture* pTex)
{
	double bullet_ang;
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);

	for (GameObject* pInst : go_list)
	{
		if (pInst->active) continue;
		pInst->type = GameObject::GO_BULLET;
		pInst->tex = pTex;
		pInst->position.x = Player->position.x;
		pInst->position.y = Player->position.y;
		pInst->scale.x = BULLET_SIZE;
		pInst->scale.y = BULLET_SIZE;
		bullet_ang = atan2(static_cast<double> (pInst->position.y - mouseY), static_cast<double> (pInst->position.x - mouseX));
		AEVec2Set(&pInst->direction, -cos(bullet_ang), -sin(bullet_ang));
		pInst->alpha = 1.0f;
		pInst->active = true;
		//std::cout << "spawned" << std::endl;
		break;
	}
}

void AOE_move(GameObject* Player, std::vector<GameObject*>& go_list, AEGfxTexture* pTex)
{
	for (GameObject* pInst : go_list)
	{
		if (pInst->active) continue;
		
		pInst->type = GameObject::GO_AOE;
		pInst->tex = pTex;
		pInst->position.x = Player->position.x;
		pInst->position.y = Player->position.y;
		pInst->scale.x = AOE_SIZE;
		pInst->scale.y = AOE_SIZE;
		pInst->alpha = AOE_ALPHA;
		AEVec2Set(&pInst->direction, 0.0f, 0.0f);
		pInst->active = true;
		break;
	}
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
		if (player->AOE.cooldown >= static_cast<f64> (5.0f)) player->AOE.on_cd = false;
	}


	if (AEInputCheckTriggered(AEVK_Z) && (player->skill_flag & shoot_flag))
	{
		return 0;
	}

	if (AEInputCheckTriggered(AEVK_X) && (player->skill_flag & AOE_flag) && !player->AOE.on_cd) 
	{
		return 1;
	}

	return -1;
}



