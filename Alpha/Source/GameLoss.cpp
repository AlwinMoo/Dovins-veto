#include "GameLoss.h"
#include "UI_Manager.h"

namespace
{
	AEGfxVertexList* pMesh;
	AEGfxTexture* player_defeat;
	AEGfxTexture* nexus_defeat;
	AEGfxTexture* Message;
	AEGfxTexture* credit_button;
	AEGfxTexture* restart_button;
	AEGfxTexture* menu_button;

	UI::UI_Manager* gameUIManager;
	//UI::UI_TextAreaTable* textTable;
}

void res_button(UI::UI_Button*);
void Credits_button(UI::UI_Button*);
void Menu_button(UI::UI_Button*);

void gameLoss_Load()
{
	player_defeat = AEGfxTextureLoad("Assets/playerLoss.png");
	nexus_defeat = AEGfxTextureLoad("Assets/nexusLoss.png");
	credit_button = AEGfxTextureLoad("Assets/CREDITS_TEX.png");
	restart_button = AEGfxTextureLoad("Assets/RESTART_TEX.png");
	menu_button = AEGfxTextureLoad("Assets/MENU_TEX.png");
}

void gameLoss_Initialize()
{
	pMesh = basic_mesh();
	gameUIManager = new UI::UI_Manager{};
	//textTable = new UI::UI_TextAreaTable;
	gameUIManager->SetWinDim(static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()));

	AEVec2 FirstPos{ static_cast<float> (AEGetWindowWidth()) * 0.5f, static_cast<float> (AEGetWindowHeight()) * 0.5f };
	AEVec2 const buildButtonStartPos{ gameUIManager->m_winDim.x * .115f, 
										gameUIManager->m_winDim.y * .9f };
	AEVec2 const buildButtonSize{ 200.f, 70.f};
	AEVec2 buildButtonPos{ buildButtonStartPos };
	gameUIManager->CreateButton(FirstPos, buildButtonSize, UI::RESTART_BUTTON,
		nullptr, res_button, nullptr); // second last param - > callback. No need hover text
	FirstPos.y -= 0.15f * static_cast<float> (AEGetWindowHeight());

	gameUIManager->CreateButton(FirstPos, buildButtonSize, UI::CREDIT_BUTTON,
		nullptr, Credits_button, nullptr); // second last param - > callback. No need hover text
	FirstPos.y -= 0.15f * static_cast<float> (AEGetWindowHeight());

	gameUIManager->CreateButton(FirstPos, buildButtonSize, UI::MENU_BUTTON,
		nullptr, Menu_button, nullptr); // second last param - > callback. No need hover text
}

void gameLoss_Update()
{

	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);

	if (AEInputCheckTriggered(AEVK_Q))
	{
		next = GS_QUIT;
	}

	AEVec2 invert_mouse = { static_cast<f32>(mouseX), static_cast<f32>(mouseY) }; // Getting inverted mouse pos to match world space
	invert_mouse.y = gameUIManager->m_winDim.y - mouseY;
	gameUIManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
}

void gameLoss_Draw()
{
	//AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

	if (playerloss)
	{
		generic_draw(pMesh, player_defeat, 1.f, 1000.f, 750.f, 0.f, 0.f);
	}

	else
		generic_draw(pMesh, nexus_defeat, 1.f, 1000.f, 750.f, 0.f, 0.f);

	s32 cursorX, cursorY;
	AEVec2 FirstPos{ 0.f, 0.f};
	AEInputGetCursorPosition(&cursorX, &cursorY);
	gameUIManager->Draw(cursorX, cursorY);

	generic_draw(pMesh, restart_button, 1.f, 200.f, 70.f, FirstPos.x, FirstPos.y);
	FirstPos.y -= 0.15f * static_cast<float> (AEGetWindowHeight());

	generic_draw(pMesh, credit_button, 1.f, 200.f, 70.f, FirstPos.x, FirstPos.y);
	FirstPos.y -= 0.15f * static_cast<float> (AEGetWindowHeight());

	generic_draw(pMesh, menu_button, 1.f, 200.f, 70.f, FirstPos.x, FirstPos.y);
}

void gameLoss_Free()
{
	AEGfxMeshFree(pMesh);
	delete gameUIManager;
	//delete textTable;
}

void gameLoss_Unload()
{
	AEGfxTextureUnload(menu_button);
	AEGfxTextureUnload(credit_button);
	AEGfxTextureUnload(restart_button);
	AEGfxTextureUnload(nexus_defeat);
	AEGfxTextureUnload(player_defeat);
}

void res_button(UI::UI_Button*)
{
	next = GS_LEVEL3;
}

void Credits_button(UI::UI_Button*)
{
	next = GS_CREDITS;
}

void Menu_button(UI::UI_Button*)
{
	next = GS_MENU;
}