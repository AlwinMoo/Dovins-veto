/******************************************************************************/
/*!
\file		Skills.cpp
\author		Bevan Lim Zhi Wei
\par        email: bevanzhiwei.lim@digipen.edu
\date       February 28, 2023
\brief		Bevan (Primary 100%)
This source file contains the necessary functions definitions for player skills,
cooldown managers and skill input management

Players have skills split into three different categories, ranged, melee and utility
which requires upgrades to be used.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Skills.h"
#include "Rendering.hpp"
#include <iostream>

namespace
{
	f32 AOE_opcty;
	f32 DASH_opcty;
	f32 CAR_opcty;
	f32 HEAL_opcty;
	int num_tiers{ 10 };

	double CAR_CD{ 10.0 };
	double DASH_CD{ 5.0 };
	double UPGRADED_DASH_CD{ 3.0 };
	double HEAL_CD{ 10.0 };
	double UPGRADED_HEAL_CD{ 5.0 };
}

void shoot_bullet(GameObject* Player, GameObject* skill_inst)
{
	double bullet_ang;
	s32 mouseX, mouseY;

	AEVec2 out{};
	AEVec2 norm{};

	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEVec2Set(&norm, (Player->position.x - mouseX), (Player->position.y - mouseY));
	AEVec2Normalize(&out, &norm);
	Player->rotation = AERadToDeg(atan2f(norm.x, norm.y));
	
	skill_inst->type = GameObject::GAMEOBJECT_TYPE::GO_BULLET;
	skill_inst->position.x = Player->position.x;
	skill_inst->position.y = Player->position.y;
	skill_inst->scale.x = skill_vals::BULLET_SIZEX;
	skill_inst->scale.y = skill_vals::BULLET_SIZEY;
	bullet_ang = atan2(static_cast<double> (skill_inst->position.y - mouseY), static_cast<double> (skill_inst->position.x - mouseX));

	AEVec2Set(&skill_inst->direction, static_cast<float>(-cos(bullet_ang)), static_cast<float>(-sin(bullet_ang)));
	AEVec2Normalize(&skill_inst->direction, &skill_inst->direction);

	if (Player->Range.skill_bit & base)
		skill_inst->Range.damage = 1.f;

	if (Player->Range.skill_bit & tier1)
		skill_inst->Range.damage += 1.f;

	//if (Player->Range.skill_bit & tier2) just to see that theres a tier but doesnt require any action

	if (Player->Range.skill_bit & tier3)
		skill_inst->Range.damage += 1.f;

	if (Player->Range.skill_bit & tier4)
	{
		skill_inst->scale.x = skill_vals::UPGRADES_BULLET_SIZEX;
		skill_inst->scale.y = skill_vals::UPGRADES_BULLET_SIZEY;
	}

	if (Player->Range.skill_bit & tier5)
		skill_inst->Range.damage += 1.f;

	//if (Player->Range.skill_bit & tier6) just to see that theres a tier but doesnt require any action

	if (Player->Range.skill_bit & tier7)
		skill_inst->Range.damage += 2.f;

	if (Player->Range.skill_bit & tier8)
		skill_inst->Range.skill_bit |= tier2;

	if (Player->Range.skill_bit & tier9)
		CAR_CD = 5.0;
}

void spreadshot(GameObject* parent, GameObject* skill_inst, int times)
{
	skill_inst->type = GameObject::GAMEOBJECT_TYPE::GO_BULLET;
	skill_inst->position.x = parent->position.x;
	skill_inst->position.y = parent->position.y;
	skill_inst->scale.x = skill_vals::BULLET_SIZEX;
	skill_inst->scale.y = skill_vals::BULLET_SIZEY;
	skill_inst->Range.damage = 1.0f;
	skill_inst->Range.skill_bit = base;
	switch(times)
	{ 
	case 0 :
		AEVec2Set(&skill_inst->direction, static_cast<float>(cos(PI)), static_cast<float>(sin(PI)));
		break;																					  
	case 1:																						  
		AEVec2Set(&skill_inst->direction, static_cast<float>(-cos(PI)), static_cast<float>(sin(PI)));
		break;
	case 2:
		AEVec2Set(&skill_inst->direction, static_cast<float>(cos(0.5 * PI)), static_cast<float>(sin(0.5 * PI)));
		break;
	case 3:
		AEVec2Set(&skill_inst->direction, static_cast<float>(cos(0.5 * PI)), static_cast<float>(-sin(0.5 * PI)));
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
	AEVec2Set(&skill_inst->direction, -cosf((float)car_ang), -sinf((float)car_ang));
}

void random_shoot(GameObject* parent, GameObject* skill_inst)
{
	skill_inst->type = GameObject::GO_BULLET;
	skill_inst->position.x = parent->position.x;
	skill_inst->position.y = parent->position.y;
	skill_inst->scale.x = skill_vals::BULLET_SIZEX;
	skill_inst->scale.y = skill_vals::BULLET_SIZEY;
	skill_inst->alpha = 1.0f;
	skill_inst->Range.damage = 1.0f;
	skill_inst->Range.skill_bit = base;
	AEVec2Set(&skill_inst->direction, -cos(2 * PI * AERandFloat()), -sin(2 * PI * AERandFloat()));
	AEVec2Normalize(&skill_inst->direction, &skill_inst->direction);
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

	if (Player->Melee.skill_bit & base)
		skill_inst->Melee.damage = 1.f;

	if (Player->Melee.skill_bit & tier1)
		skill_inst->Melee.damage += 1.f;

	if (Player->Melee.skill_bit & tier2)
	{
		skill_inst->scale.x = skill_vals::UPGRADED_AOE_SIZE;
		skill_inst->scale.y = skill_vals::UPGRADED_AOE_SIZE;
	}

	//if (Player->Range.skill_bit & tier3) just to see that theres a tier but doesnt require any action

	if (Player->Melee.skill_bit & tier4)
		skill_inst->Melee.damage += 0.5f;

	if (Player->Melee.skill_bit & tier5)
	{
		skill_inst->scale.x = skill_vals::MAX_AOE_SIZE;
		skill_inst->scale.y = skill_vals::MAX_AOE_SIZE;
	}

	if (Player->Range.skill_bit & tier6)
		skill_inst->Melee.damage += 0.5f;

	//if (Player->Range.skill_bit & tier7) just to see that theres a tier but doesnt require any action

}

void AOE_ready(GameObject* player, GameObject* AOE)
{
	//tier 4 stuff

		if (player->Melee.lifetime > 0.2 && AOE->alpha < 1.0f)
		{
			AOE->alpha += 0.1f;
			player->Melee.timer = 0;
		}

		if (AOE->alpha >= 1.0f && !AOE->skill_flag)
		{
			s32 mouseX, mouseY;
			AEInputGetCursorPosition(&mouseX, &mouseY);
			double AOE_ang;
			AEVec2 out{};
			AEVec2 norm{};

			AEVec2Set(&norm, (player->position.x - mouseX), (player->position.y - mouseY));
			AEVec2Normalize(&out, &norm);
			player->rotation = AERadToDeg(atan2f(norm.x, norm.y));

			AOE_ang = atan2(static_cast<double>(AOE->position.y - mouseY), static_cast<double> (AOE->position.x - mouseX));
			AEVec2Set(&AOE->direction, -cosf((float)AOE_ang), -sinf((float)AOE_ang));
			AEVec2 norm_direc;
			AEVec2Normalize(&norm_direc, &AOE->direction);
			AEVec2Set(&AOE->direction, norm_direc.x, norm_direc.y);
			AOE->skill_flag = true;
		}
}

void taunt_move(GameObject* player, GameObject* enemy)
{
	enemy->smallTarget = player;
}


void player_blink(GameObject* player)
{
	s32 mousex, mousey;
	AEInputGetCursorPosition(&mousex, &mousey);
	AEVec2 Mouse{ (f32)mousex, (f32)mousey };
	float dist = (player->Utility.skill_bit & tier3) ? skill_vals::UPGRADED_BLINK_RANGE : skill_vals::BLINK_RANGE;
	if (AEVec2Distance(&player->position, &Mouse) >= dist) return;

	player->position.x = (f32)mousex;
	player->position.y = (f32)mousey;

	player->Utility.first_tier.on_cd = true;
	DASH_opcty = 0.5f;
	player->Path.clear();
}


int skill_input_check(GameObject* player)
{
	if (AEInputCheckCurr(AEVK_LBUTTON) && (player->Range.skill_bit & base) == base && !player->Range.first_tier.on_cd)
	{
		player->Range.first_tier.on_cd = true;
		return shooting;
	}

	if (AEInputCheckTriggered(AEVK_X) && (player->Melee.skill_bit & base) == base && !player->Melee.first_tier.on_cd)  //AOE
	{
		player->Melee.first_tier.active = true;
		player->Melee.first_tier.on_cd = true;
		AOE_opcty = 0.5f;
		return AOEing;
	}

	if (AEInputCheckTriggered(AEVK_C) && (player->Range.skill_bit & tier9) && !player->Range.second_tier.on_cd) //car
	{
		player->Range.second_tier.active = true;
		player->Range.second_tier.on_cd = true;
		return car;
	}

	if (AEInputCheckTriggered(AEVK_V) && (player->Utility.skill_bit & tier1))
	{
		player->Utility.second_tier.on_cd = true;
		HEAL_opcty = 0.5f;
		return heal;
	}

	if (AEInputCheckTriggered(AEVK_LSHIFT) && player->Utility.skill_bit & base)
	{
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
	clone->rotation = player->rotation;
}

//skill UI stuff
void cooldown_check(GameObject* player)
{
	if (player->Melee.first_tier.on_cd) //AOE cooldown
	{
		player->Melee.first_tier.cooldown += AEFrameRateControllerGetFrameTime();
		double timer;
		timer = (player->Melee.skill_bit & tier3) ? 5.0 : 7.5;
		AOE_opcty += 0.5f / (f32)(timer / AEFrameRateControllerGetFrameTime());
		if (player->Melee.first_tier.cooldown >= timer)
		{
			player->Melee.first_tier.cooldown = 0.0;
			player->Melee.first_tier.on_cd = false;
			AOE_opcty = 1.f;
		}
	}

	if (player->Range.second_tier.on_cd) //car cooldown
	{
		player->Range.second_tier.cooldown += AEFrameRateControllerGetFrameTime();
		CAR_opcty += 0.5f / (f32)(CAR_CD/ AEFrameRateControllerGetFrameTime());
		if (player->Range.second_tier.cooldown >= CAR_CD)
		{
			player->Range.second_tier.cooldown = 0.0;
			player->Range.second_tier.on_cd = false;
			CAR_opcty = 1.f;

		}
	}

	if (player->Range.first_tier.on_cd) //shoot cooldown
	{
		player->Range.first_tier.cooldown += AEFrameRateControllerGetFrameTime();
		if (player->Range.first_tier.cooldown >= player->Range.timer)
		{
			player->Range.first_tier.cooldown = 0.0;
			player->Range.first_tier.on_cd = false;
		}
	}

	if (player->Utility.first_tier.on_cd)
	{
		player->Utility.first_tier.cooldown += AEFrameRateControllerGetFrameTime();
		double timer;
		timer = (player->Utility.skill_bit & tier1) ? UPGRADED_DASH_CD : DASH_CD;
		DASH_opcty += 0.5f / (f32)(timer / AEFrameRateControllerGetFrameTime());
		if (player->Utility.first_tier.cooldown >= timer)
		{
			player->Utility.first_tier.cooldown = 0.0;
			player->Utility.first_tier.on_cd = false;
		}
	}

	if (player->Utility.second_tier.on_cd)
	{
		player->Utility.second_tier.cooldown += AEFrameRateControllerGetFrameTime();
		double timer = (player->Utility.skill_bit & tier4) ? UPGRADED_HEAL_CD : HEAL_CD;
		HEAL_opcty += 0.5f / (f32)(timer / AEFrameRateControllerGetFrameTime());
		if (player->Utility.second_tier.cooldown >= timer)
		{
			player->Utility.second_tier.cooldown = 0.0;
			player->Utility.second_tier.on_cd = false;
		}
	}
}

void cooldown_UI(GameObject* player, AEGfxVertexList* pMesh)
{
	if (player->Melee.skill_bit & base)
	{
		if (!player->Melee.first_tier.on_cd)
		{
			AOE_opcty = 1.f;
			generic_draw(pMesh, UI::TextureList[UI::UI_TEX::TEX_READY], 1.f, 350.f, 300.f, -450.f, -325.f);
		}

		generic_draw(pMesh, UI::TextureList[UI::UI_TEX::TEX_AOE_SKILL], AOE_opcty, 100.f, 100.f, -450.f, -400.f);
	}
		

	if (player->Utility.skill_bit & base)
	{
		if (!player->Utility.first_tier.on_cd)
		{
			DASH_opcty = 1.f;
			generic_draw(pMesh, UI::TextureList[UI::UI_TEX::TEX_READY], 1.f, 350.f, 300.f, -300.f, -325.f);
		}
		generic_draw(pMesh, UI::TextureList[UI::UI_TEX::TEX_SKILL_MENU], DASH_opcty, 100.f, 100.f, -300.f, -400.f);
	}

	if (player->Utility.skill_bit & tier1)
	{
		if (!player->Utility.second_tier.on_cd)
		{
			HEAL_opcty = 1.f;
			generic_draw(pMesh, UI::TextureList[UI::UI_TEX::TEX_READY], 1.f, 350.f, 300.f, -0.f, -325.f);
		}
		generic_draw(pMesh, UI::TextureList[UI::UI_TEX::TEX_HEAL_SKILL], HEAL_opcty, 100.f, 100.f, 0.f, -400.f);
	}

	if (player->Range.skill_bit & tier9)
	{
		if (!player->Range.second_tier.on_cd)
		{
			CAR_opcty = 1.f;
			generic_draw(pMesh, UI::TextureList[UI::UI_TEX::TEX_READY], 1.f, 350.f, 300.f, -150.f, -325.f);
		}

		generic_draw(pMesh, UI::TextureList[UI::UI_TEX::TEX_GUN_SKILL], CAR_opcty, 100.f, 100.f, -150.f, -400.f);

	}
		
}

void cooldown_reset(GameObject* player)
{
	player->Melee.first_tier.on_cd = false;
	player->Range.second_tier.on_cd = false;
	player->Utility.first_tier.on_cd = false;
	player->Utility.second_tier.on_cd = false;
}


