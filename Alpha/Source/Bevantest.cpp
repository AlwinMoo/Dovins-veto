#include "Bevantest.h"
#include "AEEngine.h"
#include <iostream>
#include <iomanip>
#include "UI_Manager.h"
f32 winSizeX, winSizeY;
s8 m_fontId;

static UI::UI_Manager uiManager;

AEGfxTexture* pTex;
AEGfxVertexList* pMesh;
s32 click_pos_x;
s32 click_pos_y;
s32 pos_x, pos_y;
s32 x, y;
void Bevantest_Load()
{
	pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	m_fontId = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 16);
}

void Bevantest_Initialize()
{
	winSizeX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	winSizeY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	pos_x = 400;
	pos_y = 300;
	click_pos_x = 400;
	click_pos_y = 300;
	x = 0;
	y = 0;
	pMesh = 0;
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	pMesh = AEGfxMeshEnd();

	
}

void Bevantest_Update()
{
	// IMPORTANT SNIPPET:
		/***********************************************************************/
	s32 cursorX, cursorY;
	AEInputGetCursorPosition(&cursorX, &cursorY);
	f32 winX{ AEGfxGetWinMaxX() - AEGfxGetWinMinX() }, winY{ AEGfxGetWinMaxY() - AEGfxGetWinMinY() };
	//std::cout << winX << ", " <<  winY << std::endl;
	f32 cursorXN{ cursorX / winX * 2 - 1.f }, cursorYN{ cursorY / winY * -2 + 1.f }; // NORMALIZED COORDINATES
	std::cout << std::setprecision(2) << std::setw(5) << cursorXN << ", " << std::setw(5) << cursorYN << std::endl;
	/************************************************************************/
	s8 const* testStr = "TEST TEXT";
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(m_fontId, (s8*)testStr, cursorXN, cursorYN, 2.f, 1.f, 0.f, 0.f);

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	if (AEInputCheckTriggered(AEVK_RBUTTON))
	{
		AEInputGetCursorPosition(&click_pos_x, &click_pos_y);
		//click_check(click_pos_x, click_pos_y);
		//click_check(pos_x, pos_y);
	}
	// Your own update logic goes here
	if (pos_x < click_pos_x)
	{
		x += 2;
		pos_x += 2;
	}

	else if (pos_x > click_pos_x)
	{
		x -= 2;
		pos_x -= 2;
	}

	if (pos_y < click_pos_y)
	{
		y -= 2;
		pos_y += 2;
	}

	else if (pos_y > click_pos_y)
	{
		y += 2;
		pos_y -= 2;
	}

	// Your own rendering logic goes here
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(pTex, 0, 0);

	//scale matrix
	AEMtx33 scale{ 0 };
	AEMtx33Scale(&scale, 150.0f, 150.0f);

	//translation matrix
	AEMtx33 translate{ 0 };
	AEMtx33Trans(&translate, x, y);

	//rotation matrix
	AEMtx33 rotation{ 0 };
	AEMtx33Rot(&rotation, 100.0f);

	AEMtx33 transform{ 0 };
	AEMtx33Concat(&transform, &rotation, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	//setting the transform
	AEGfxSetTransform(transform.m);
}

void Bevantest_Draw()
{
	//Draw mesh
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void Bevantest_Free()
{
	AEGfxMeshFree(pMesh);
}

void Bevantest_Unload()
{
	AEGfxTextureUnload(pTex);
}