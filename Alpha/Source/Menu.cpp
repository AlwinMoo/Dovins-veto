#include "Menu.h"

namespace
{
	AEGfxTexture* pTex;
	AEGfxVertexList* pMesh;
}

void menu_Load()
{
	pTex = AEGfxTextureLoad("Assets/EnemyTexture.png");
}

void menu_Initialize()
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
}

void menu_Update()
{
	if (AEInputCheckTriggered(AEVK_Q))
	{
		next = GS_QUIT;
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		next = GS_LEVEL3;
	}
}

void menu_Draw()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(pTex, 0, 0);

	AEMtx33 scale = { 0 };
	AEMtx33 rotate = { 0 };
	AEMtx33 translate = { 0 };
	AEMtx33 transform = { 0 };

	AEMtx33Scale(&scale, 250.f, 250.f);
	AEMtx33Rot(&rotate, 0);
	AEMtx33Trans(&translate, -300.f, 200.f);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void menu_Free()
{
	AEGfxMeshFree(pMesh);
}

void menu_Unload()
{
	AEGfxTextureUnload(pTex);
}