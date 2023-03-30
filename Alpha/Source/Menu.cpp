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

//button declaration
void start_button(UI::UI_Button*);
void tutorial_button(UI::UI_Button*);
void credits_button(UI::UI_Button*);
void exit_button(UI::UI_Button*);

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
		AEVec2 buttonPos{ gameUIManager->m_winDim.x * 0.5f, gameUIManager->m_winDim.y * 0.5f };
		AEVec2 const buttonSize{ 200.f, 70.f };
		// PLAY BUTTON
		gameUIManager->CreateButton(buttonPos, buttonSize, UI::SKILL_TREE_BUTTON,
			&textTable->playButton, start_button, nullptr);
		buttonPos.y -= yOffset;
		// HOW TO PLAY
		gameUIManager->CreateButton(buttonPos, buttonSize, UI::SKILL_TREE_BUTTON,
			&textTable->howToButton, nullptr, nullptr);
		// CREDITS
		buttonPos.y -= yOffset;
		gameUIManager->CreateButton(buttonPos, buttonSize, UI::SKILL_TREE_BUTTON,
			&textTable->creditsButton, credits_button, nullptr);
		// QUIT
		buttonPos.y -= yOffset;
		gameUIManager->CreateButton(buttonPos, buttonSize, UI::SKILL_TREE_BUTTON,
			&textTable->quitButton, exit_button, nullptr);
	}

	pMesh = basic_mesh();
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
	generic_draw(pMesh, pTex, 1.0f, 250.f, 250.f, -300.f, 200.f);

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

void start_button(UI::UI_Button*)
{
	next = GS_LEVEL3;
}
void tutorial_button(UI::UI_Button*)
{
	//next = tutorial scene
}
void credits_button(UI::UI_Button*)
{
	next = GS_CREDITS;
}

void exit_button(UI::UI_Button*)
{
	next = GS_QUIT;
}

void generic_draw(AEGfxVertexList* mesh, AEGfxTexture* tex, f32 opacity, f32 width, f32 height, f32 x, f32 y)
{

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, opacity);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(tex, 0, 0);

	AEMtx33 scale = { 0 };
	AEMtx33 rotate = { 0 };
	AEMtx33 translate = { 0 };
	AEMtx33 transform = { 0 };

	AEMtx33Scale(&scale, width, height);
	AEMtx33Rot(&rotate, 0);
	AEMtx33Trans(&translate, x, y);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

AEGfxVertexList* basic_mesh()
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

	return AEGfxMeshEnd();
}