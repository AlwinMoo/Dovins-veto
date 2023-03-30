#include "Splashscreen.h"
#include "Menu.h"

namespace
{
	AEGfxTexture*		logo;
	AEGfxTexture*		bTex;
	AEGfxTexture*		aTex;
	AEGfxTexture*		dTex;
	AEGfxTexture*		eTex;
	AEGfxTexture*		twoTex;

	AEGfxVertexList* logo_mesh;

	f32					digi_opacity;
	f32					logo_pt1;
	f32					logo_pt2;
	f32					logo_pt3;
	f32					logo_pt4;
	f32					logo_pt5;
	f32					logo_pt6;

	f64					dt;
	f64					timer;
	bool				digi_logo;

	bool				gam_logo1;
	bool				gam_logo2;
	bool				gam_logo3;
	bool				gam_logo4;
	bool				gam_logo5;
	bool				gam_logo6;
	bool				time_stall;
	
	f32					win_X;
	f32					win_Y;
}

void splashscreen_Load()
{
	logo = AEGfxTextureLoad("Assets/DigiPen_Logo_2023.png");
	bTex = AEGfxTextureLoad("Assets/B_Badea2.png");
	aTex = AEGfxTextureLoad("Assets/A_Badea2.png");
	dTex = AEGfxTextureLoad("Assets/D_Badea2.png");
	eTex = AEGfxTextureLoad("Assets/E_Badea2.png");
	twoTex = AEGfxTextureLoad("Assets/2_Badea2.png");
}

void splashscreen_Initialize()
{

	logo_mesh = basic_mesh();
	
	win_X = static_cast<f32>(AEGetWindowWidth());
	win_Y = static_cast<f32>(AEGetWindowHeight());

	digi_opacity = 1.0f;
	logo_pt1 = 0.f;
	logo_pt2 = 0.f;
	logo_pt3 = 0.f;
	logo_pt4 = 0.f;
	logo_pt5 = 0.f;
	logo_pt6 = 0.f;

	digi_logo = true;
	gam_logo1 = false;
	gam_logo2 = false;
	gam_logo3 = false;
	gam_logo4 = false;
	gam_logo5 = false;
	gam_logo6 = false;
	time_stall = false;
}

void splashscreen_Update()
{
	dt = AEFrameRateControllerGetFrameTime();

	timer += dt;
	if (timer > 1.0 && digi_logo)
	{
		digi_opacity -= 0.1f;
		if (digi_opacity < 0.1f)
		{
			digi_logo = false;
			gam_logo1 = true;
			timer = 0;
		}
	}

	if (timer > 0.5 && gam_logo1 && !gam_logo2)
	{
		logo_pt1 += 0.1f;

		if (logo_pt1 > 1.f)
		{
			timer = 0;
			logo_pt1 = 1.f;
			gam_logo2 = true;
		}
	}

	if (timer > 0.5 && gam_logo2 && !gam_logo3)
	{
		logo_pt2 += 0.1f;

		if (logo_pt2 > 1.f)
		{
			timer = 0;
			logo_pt2 = 1.f;
			gam_logo3 = true;
		}
	}

	if (timer > 0.5 && gam_logo3 && !gam_logo4)
	{
		logo_pt3 += 0.1f;

		if (logo_pt3 > 1.f)
		{
			timer = 0;
			logo_pt3 = 1.f;
			gam_logo4 = true;
		}
	}

	if (timer > 0.5 && gam_logo4 && !gam_logo5)
	{
		logo_pt4 += 0.1f;

		if (logo_pt4 > 1.f)
		{
			timer = 0;
			logo_pt4 = 1.f;
			gam_logo5 = true;
		}
	}

	if (timer > 0.5 && gam_logo5 && !gam_logo6)
	{
		logo_pt5 += 0.1f;

		if (logo_pt5 > 1.f)
		{
			timer = 0;
			logo_pt5 = 1.f;
			gam_logo6 = true;
		}
	}

	if (timer > 0.5 && gam_logo6 && !time_stall)
	{
		logo_pt6 += 0.1f;

		if (logo_pt6 > 1.f)
		{
			timer = 0;
			logo_pt6 = 1.f;
			time_stall = true;
		}
	}

	if (timer > 1.0 && !digi_logo && gam_logo1 && gam_logo2 && gam_logo3 && gam_logo4 && gam_logo5 && gam_logo6 && time_stall)
	{
		logo_pt1 -= 0.1f;
		logo_pt2 -= 0.1f;
		logo_pt3 -= 0.1f;
		logo_pt4 -= 0.1f;
		logo_pt5 -= 0.1f;
		logo_pt6 -= 0.1f;

		if(logo_pt1 <= 0)
			next = GS_MENU;
	}

	if (AEInputCheckTriggered(AEVK_Q))
	{
		next = GS_QUIT;
	}

	if (AEInputCheckTriggered(AEVK_N))
	{
		next = GS_MENU;
	}
}

void splashscreen_Draw()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	if (digi_logo)
		generic_draw(logo_mesh, logo, digi_opacity, win_X, win_Y, 0, 0);

	if (gam_logo1)
		generic_draw(logo_mesh, bTex, logo_pt1, 1000.f, 1200.f, -200.f, -50.f);

	if (gam_logo2)
		generic_draw(logo_mesh, aTex, logo_pt2, 1000.f, 1200.f, -150.f, -50.f);

	if (gam_logo3)
		generic_draw(logo_mesh, dTex, logo_pt3, 1000.f, 1200.f, -100.f, -50.f);

	if (gam_logo4)
		generic_draw(logo_mesh, eTex, logo_pt4, 1000.f, 1200.f, -50.f, -50.f);

	if (gam_logo5)
		generic_draw(logo_mesh, aTex, logo_pt5, 1000.f, 1200.f, 0.f, -50.f);

	if (gam_logo6)
		generic_draw(logo_mesh, twoTex, logo_pt6, 1000.f, 1200.f, 100.f, -50.f);
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
	AEGfxTextureUnload(twoTex);
	AEGfxTextureUnload(eTex);
	AEGfxTextureUnload(dTex);
	AEGfxTextureUnload(aTex);
	AEGfxTextureUnload(bTex);
	AEGfxTextureUnload(logo);
}