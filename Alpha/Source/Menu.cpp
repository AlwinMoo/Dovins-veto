#include "Menu.h"
#include "UI_Manager.h"
#include "UI_TextAreaTable.h"
namespace
{
	AEGfxTexture* pTex;
	AEGfxVertexList* pMesh;

	UI::UI_Manager* gameUIManager;
	UI::UI_TextAreaTable* textTable;
}

void menu_Load()
{
	pTex = AEGfxTextureLoad("Assets/EnemyTexture.png");
}

void menu_Initialize()
{
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenHeightY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		//auto meshTest = render::GenerateQuad();
		gameUIManager = new UI::UI_Manager();
		textTable = new UI::UI_TextAreaTable;
		gameUIManager->SetWinDim(screenWidthX, screenHeightY);


		float const yOffset{ gameUIManager->m_winDim.y * 0.1f };
		AEVec2 buttonPos{ gameUIManager->m_winDim.x * 0.5f, gameUIManager->m_winDim.y * 0.6f };
		AEVec2 const buttonSize{ 200.f, 70.f };
		// PLAY BUTTON
		gameUIManager->CreateButton(buttonPos, buttonSize, UI::SKILL_TREE_BUTTON,
			&textTable->playButton, nullptr, nullptr);
		buttonPos.y -= yOffset;
		// HOW TO PLAY
		gameUIManager->CreateButton(buttonPos, buttonSize, UI::SKILL_TREE_BUTTON,
			&textTable->howToButton, nullptr, nullptr);
		// CREDITS
		buttonPos.y -= yOffset;
		gameUIManager->CreateButton(buttonPos, buttonSize, UI::SKILL_TREE_BUTTON,
			&textTable->creditsButton, nullptr, nullptr);
		// QUIT
		buttonPos.y -= yOffset;
		gameUIManager->CreateButton(buttonPos, buttonSize, UI::SKILL_TREE_BUTTON,
			&textTable->quitButton, nullptr, nullptr);
	}
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
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);

	if (AEInputCheckTriggered(AEVK_Q))
	{
		next = GS_QUIT;
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		next = GS_LEVEL3;
	}

	AEVec2 invert_mouse = { static_cast<f32>(mouseX), static_cast<f32>(mouseY) }; // Getting inverted mouse pos to match world space
	invert_mouse.y = gameUIManager->m_winDim.y - mouseY;
	gameUIManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
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

	s32 cursorX, cursorY;
	AEInputGetCursorPosition(&cursorX, &cursorY);
	gameUIManager->Draw(cursorX, cursorY);
}

void menu_Free()
{
	AEGfxMeshFree(pMesh);
	delete gameUIManager;
	delete textTable;
}

void menu_Unload()
{
	AEGfxTextureUnload(pTex);
}