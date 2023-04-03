/******************************************************************************/
/*!
\file		GameObject.cpp
\author		Alwin Moo
\author		Alvin Yeo
\par        email: moo.g@digipen.edu
\par		email: a.yeo@digipen.edu
\date       January 10, 2023
\brief		Alwin (Primary 60%), Alvin (Secondary 40%)
			GameObject effector
			Alwin: update
			Alvin: render

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "GameObject.h"
#include "Rendering.hpp"
#include <iostream>
#include <AEEngine.h>

void GameObject::Update()
{
	if (active)
	{
		switch (type)
		{
			case (GameObject::GAMEOBJECT_TYPE::GO_ENEMY):
			{

				AEVec2 result{ 0,0 };
				AEVec2Sub(&result, &target_pos, &position);
				rotation = AERadToDeg(atan2f(result.x, result.y)); // rotate to face player

				if (!Path.empty()) // applying pathfinding to movement
				{
					AEVec2 out{};
					AEVec2 norm{};
					AEVec2Set(&norm, (Path[0].x - position.x), (Path[0].y - position.y));
					AEVec2Normalize(&out, &norm);

					position.x += out.x * static_cast<float>(AEFrameRateControllerGetFrameTime()) * Stats.GetStat(STAT_MOVE_SPEED);
					position.y += out.y * static_cast<float>(AEFrameRateControllerGetFrameTime()) * Stats.GetStat(STAT_MOVE_SPEED);


					AEVec2 leng{};
					leng.x = position.x - Path[0].x;
					leng.y = position.y - Path[0].y;

					if (AEVec2Length(&leng) <= 5)
					{
						Path.erase(Path.begin());
					}
				}
			}
			break;
		}
	}
}

void GameObject::Render()
{
	if (tex)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(tex, 0, 0);	
	}
	else
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxTextureSet(NULL, 0, 0);

	}
	AEGfxSetTintColor(color.r, color.g, color.b, 1.f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(alpha);

	AEMtx33 s = { 0 };
	AEMtx33Scale(&s, this->scale.x, this->scale.x);

	AEMtx33 r = { 0 };
	AEMtx33Rot(&r, AEDegToRad(static_cast<float>(rotation) + 180.f));

	AEMtx33 t = { 0 };
	AEMtx33Trans(&t, position.x - AEGetWindowWidth() / 2.f, -position.y + AEGetWindowHeight() / 2.f);

	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &r, &s);
	AEMtx33Concat(&transform, &t, &transform);

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

}

GameObject::GameObject(GAMEOBJECT_TYPE default_type) :
	type(default_type), active(false), rotation(0), direction()
{
	tex = nullptr;
	mesh = render::GenerateQuad();
	alpha = 1.0f;
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	position = { 0, 0 };
	scale = { 1, 1 };
	gridScale = { 1, 1 };
	
}

GameObject::~GameObject()
{	
	if(mesh) AEGfxMeshFree(mesh);
}

void Color::Set(float red, float green, float blue)
{
	this->r = red;
	this->g = green;
	this->b = blue;
}
