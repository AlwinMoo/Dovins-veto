/******************************************************************************/
/*!
\file		Skills.cpp
\author		Bevan Lim Zhi Wei
\par        email: bevanzhiwei.lim\@digipen.edu
\date       March 30, 2023
\brief		Bevan (Primary 100%)
This source file contains the necessary functions definitions for the execution
of the credits sequence for Badea 2

The credits sequence displays the people who made the game as well as our thanks to
those who have supported us

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Credits.h"

namespace
{
	AEGfxVertexList*	pMesh;
	AEGfxTexture*		teamTex;
	AEGfxTexture*		executivesTex;
	AEGfxTexture*		instructorsTex;
	AEGfxTexture*		logoTex;
	AEGfxTexture*		thanksTex;
	AEGfxTexture*		enemyTex;
	AEGfxTexture*		enemy_nexusTex;
	AEGfxTexture*		tankTex;

	bool logo;
	bool team;
	bool instructors;
	bool executives;
	bool thanks;

	f32 logo_opcty;
	f32 thanks_opcty;

	f32 winX;
	f32 winY;

	f64 dt;
	f64 timer;

	const double TEAM_TIME			{ 2.5 };
	const double INSTRUCTORS_TIME	{ 2.5 };
	const double EXECUTIVES_TIME	{ 2.5 };
}
void credits_Load()
{
	teamTex			= AEGfxTextureLoad("Assets/Badea2_Credits.png");
	executivesTex	= AEGfxTextureLoad("Assets/Executives_Credits.png");
	instructorsTex	= AEGfxTextureLoad("Assets/Credits_Instructors.png");
	logoTex			= AEGfxTextureLoad("Assets/DigiPen_Logo_2023.png");
	thanksTex		= AEGfxTextureLoad("Assets/thanks_for_playing.png");
	enemyTex		= AEGfxTextureLoad("Assets/EnemyTexture.png");
	enemy_nexusTex	= AEGfxTextureLoad("Assets/nexus_enemy.png");
	tankTex			= AEGfxTextureLoad("Assets/tank.png");
}

void credits_Initialize()
{
	AEGfxMeshStart();

	AEGfxTriAdd(
		0.5f, 0.5f, 0xFFFF00FF, 1.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF00FFFF, 1.0f, 1.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	pMesh = AEGfxMeshEnd();

	winX = static_cast<f32> (AEGetWindowWidth());
	winY = static_cast<f32> (AEGetWindowHeight());

	logo			= true;
	team			= false;
	instructors		= false;
	executives		= false;
	thanks			= false;

	logo_opcty		= 1.0f;
	thanks_opcty	= 1.0f;

	timer = 0;
}

void credits_Update()
{
	dt = AEFrameRateControllerGetFrameTime();

	timer += dt;

	if (logo && timer >= 1.0)
	{
		logo_opcty -= 0.1f;

		if (logo_opcty <= 0.1f)
		{
			timer = 0;
			logo = false;
			team = true;
		}
	}

	if (team && timer >= TEAM_TIME)
	{
		timer = 0;
		team = false;
		instructors = true;
	}

	if (instructors && timer >= INSTRUCTORS_TIME)
	{
		timer = 0;
		instructors = false;
		executives = true;
	}

	if (executives && timer >= EXECUTIVES_TIME)
	{
		timer = 0;
		executives = false;
		thanks = true;
	}

	if (thanks)
	{
		if (timer >= 1.0)
			thanks_opcty -= 0.1f;

		if (thanks_opcty <= 0.1f)
			thanks = false;
	}

	if (!thanks && !executives && !instructors && !team && !logo)
	{
		next = GS_MENU;
	}
}

void credits_Draw()
{
	if (logo)
		generic_draw(pMesh, logoTex, logo_opcty, winX, winY, 0.f, 0.f);

	if (team)
		generic_draw(pMesh, teamTex, 1.0f, winX, winY, 0.f, 0.f);

	if (instructors)
		generic_draw(pMesh, instructorsTex, 1.0f, winX, winY, 0.f, 0.f);

	if (executives)
		generic_draw(pMesh, executivesTex, 1.0f, winX, winY, 0.f, 0.f);

	if (thanks)
	{
		generic_draw(pMesh, thanksTex, thanks_opcty, winX, winY, 0.f, 0.f);
		generic_draw(pMesh, enemyTex, thanks_opcty, 200.f, 200.f, -450.f, 400.f);
		generic_draw(pMesh, enemy_nexusTex, thanks_opcty, 200.f, 200.f, 450.f, 400.f);
		generic_draw(pMesh, tankTex, thanks_opcty, 200.f, 200.f, 0.f, -300.f);
	}
}

void credits_Free()
{
	AEGfxMeshFree(pMesh);
}

void credits_Unload()
{
	AEGfxTextureUnload(tankTex);
	AEGfxTextureUnload(enemy_nexusTex);
	AEGfxTextureUnload(enemyTex);
	AEGfxTextureUnload(thanksTex);
	AEGfxTextureUnload(executivesTex);
	AEGfxTextureUnload(instructorsTex);
	AEGfxTextureUnload(teamTex);
	AEGfxTextureUnload(logoTex);
}