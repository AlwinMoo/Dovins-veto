#include "Splashscreen.h"
#include "Menu.h"

namespace
{
	AEGfxTexture*		logo;
	AEGfxVertexList*	logo_mesh;
	//AEGfxTexture*		badea2;
	//AEGfxVertexList*	badea2_mesh
	f32					digi_opacity			{ 1.0f };
	f64					dt;
	f64					timer;
	bool				digi_logo		{ true };
	bool				gam_logo		{ false };
	
	f32					win_X;
	f32					win_Y;
}

void splashscreen_Load()
{
	logo = AEGfxTextureLoad("Assets/DigiPen_Logo_2023.png");
}

void splashscreen_Initialize()
{
	AEGfxMeshStart();

	AEGfxTriAdd(
		 0.5f,  0.5f, 0xFFFF00FF, 1.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
		 0.5f, -0.5f,  0xFF00FFFF, 1.0f, 1.0f);

	AEGfxTriAdd(
		-0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		 0.5f,  0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	logo_mesh = AEGfxMeshEnd();
	
	win_X = static_cast<f32>(AEGetWindowWidth());
	win_Y = static_cast<f32>(AEGetWindowHeight());
}

void splashscreen_Update()
{
	dt = AEFrameRateControllerGetFrameTime();

	timer += dt;
	if (timer > 1.0)
	{
		digi_opacity -= 0.1f;
		if (digi_opacity < 0.1f && digi_logo)
		{
			digi_logo = false;
			timer = 0;
		}
	}

	if (AEInputCheckTriggered(AEVK_Q))
	{
		next = GS_QUIT;
	}

	if (!digi_logo && !gam_logo)
	{
		next = GS_MENU;
	}
}

void splashscreen_Draw()
{
	if (digi_logo)
		generic_draw(logo_mesh, logo, digi_opacity, win_X, win_Y, 0, 0);

	//if (gam_logo)
	//{
	//	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, opacity);
	//	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//	AEGfxSetTransparency(1.0f);

	//	AEGfxTextureSet(badea2, 0, 0);

	//	AEMtx33 scale = { 0 };
	//	AEMtx33 rotate = { 0 };
	//	AEMtx33 translate = { 0 };
	//	AEMtx33 transform = { 0 };

	//	AEMtx33Scale(&scale, AEGetWindowWidth(), AEGetWindowHeight());
	//	AEMtx33Rot(&rotate, 0);
	//	AEMtx33Trans(&translate, 0, 0);
	//	AEMtx33Concat(&transform, &rotate, &scale);
	//	AEMtx33Concat(&transform, &translate, &transform);

	//	AEGfxSetTransform(transform.m);
	//	AEGfxMeshDraw(logo_mesh, AE_GFX_MDM_TRIANGLES);
	//}

}

void splashscreen_Free()
{
	AEGfxMeshFree(logo_mesh);
}

void splashscreen_Unload()
{
	AEGfxTextureUnload(logo);
}