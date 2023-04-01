#include "GameLoss.h"
#include "UI_Manager.h"

namespace
{
	AEGfxVertexList* pMesh;
	AEGfxTexture* player_defeat;
	AEGfxTexture* nexus_defeat;
	AEGfxTexture* Message;
	AEGfxTexture* credit_button;

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
}

void gameLoss_Initialize()
{
	pMesh = basic_mesh();
	gameUIManager = new UI::UI_Manager{};
	//textTable = new UI::UI_TextAreaTable;
	gameUIManager->SetWinDim(AEGetWindowWidth(), AEGetWindowHeight());

	AEVec2 FirstPos{ AEGetWindowWidth() * 0.5f, AEGetWindowHeight() * 0.5 };
	AEVec2 const buildButtonStartPos{ gameUIManager->m_winDim.x * .115f, 
										gameUIManager->m_winDim.y * .9f };
	AEVec2 const buildButtonSize{ 200.f, 70.f};
	AEVec2 buildButtonPos{ buildButtonStartPos };
	gameUIManager->CreateButton(FirstPos, buildButtonSize, UI::RESTART_BUTTON,
		nullptr, res_button, nullptr); // second last param - > callback. No need hover text
	FirstPos.y -= 0.15f * AEGetWindowHeight();

	gameUIManager->CreateButton(FirstPos, buildButtonSize, UI::CREDIT_BUTTON,
		nullptr, Credits_button, nullptr); // second last param - > callback. No need hover text
	FirstPos.y -= 0.15f * AEGetWindowHeight();

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
	AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

	if (playerloss)
	{
		generic_draw(pMesh, player_defeat, 1.f, 1000.f, 750.f, 0.f, 0.f);
	}

	else
		generic_draw(pMesh, nexus_defeat, 1.f, 1000.f, 750.f, 0.f, 0.f);

	s32 cursorX, cursorY;
	AEInputGetCursorPosition(&cursorX, &cursorY);
	gameUIManager->Draw(cursorX, cursorY);
}

void gameLoss_Free()
{
	AEGfxMeshFree(pMesh);
	delete gameUIManager;
	//delete textTable;
}

void gameLoss_Unload()
{
	AEGfxTextureUnload(nexus_defeat);
	AEGfxTextureUnload(player_defeat);
}

void res_button(UI::UI_Button*)
{
	next = GS_RESTART;
}

void Credits_button(UI::UI_Button*)
{
	next = GS_CREDITS;
}

void Menu_button(UI::UI_Button*)
{
	next = GS_MENU;
}