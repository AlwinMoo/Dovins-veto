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
				if (!Path.empty()) // applying pathfinding to movement
				{
					AEVec2 out{};
					AEVec2 norm{};
					AEVec2Set(&norm, (Path[0].x - position.x), (Path[0].y - position.y));
					AEVec2Normalize(&out, &norm);

					position.x += out.x * AEFrameRateControllerGetFrameTime() * 100;
					position.y += out.y * AEFrameRateControllerGetFrameTime() * 100;


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
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(color.r, color.g, color.b, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(alpha);
	AEGfxTextureSet(tex, 0, 0);

	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, this->scale.x, this->scale.x);

	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, AEDegToRad(rotation + 180.f));

	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, position.x - AEGetWindowWidth() / 2.f, -position.y + AEGetWindowHeight() / 2.f);

	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

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
	AEGfxMeshFree(mesh);
}

void Color::Set(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}
