#include "GameObject.h"
#include "Rendering.hpp"
#include <iostream>

void GameObject::Update()
{
}

void GameObject::Render()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(tex, 0, 0);

	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, this->scale.x, this->scale.x);

	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, rotation);

	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, position.x - AEGetWindowWidth() / 2.f, -position.y + AEGetWindowHeight() / 2.f);

	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

}

GameObject::GameObject(GAMEOBJECT_TYPE default_type) :
	type(default_type), active(false), rotation(0)
{
	tex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	mesh = render::GenerateQuad();
	position = { 0, 0 };
	scale = { 1, 1 };

}

GameObject::~GameObject()
{
	AEGfxMeshFree(mesh);
	AEGfxTextureUnload(tex);
}
