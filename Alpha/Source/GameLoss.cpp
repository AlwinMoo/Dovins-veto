#include "GameLoss.h"

namespace
{
	AEGfxVertexList* pMesh;
	AEGfxTexture* player_defeat;
	AEGfxTexture* nexus_defeat;
	AEGfxTexture* Message;

	UI::UI_Manager* gameUIManager;
}

void gameLoss_Load()
{
	player_defeat = AEGfxTextureLoad("Assets/playerLoss.png");
	nexus_defeat = AEGfxTextureLoad("Assets/nexusLoss.png");
}

void gameLoss_Initialize()
{
	pMesh = basic_mesh();
	gameUIManager = new UI::UI_Manager{};
	gameUIManager->SetWinDim(AEGetWindowWidth(), AEGetWindowHeight());
	AEVec2 const buildButtonStartPos{ gameUIManager->m_winDim.x * .115f, 
										gameUIManager->m_winDim.y * .9f };
	AEVec2 const buildButtonSize{ gameUIManager->m_winDim.y * 0.2f, gameUIManager->m_winDim.y * .1f };
	AEVec2 buildButtonPos{ buildButtonStartPos };
	gameUIManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_NEXUS_BUTTON,
		nullptr, nullptr, nullptr); // second last param - > callback. No need hover text
	buildButtonPos.x -= buildButtonSize.y * 0.75f;
	gameUIManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_NEXUS_BUTTON,
		nullptr, nullptr, nullptr); // second last param - > callback. No need hover text
	buildButtonPos.x -= buildButtonSize.y * 0.75f;
	gameUIManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_NEXUS_BUTTON,
		nullptr, nullptr, nullptr); // second last param - > callback. No need hover text
}

void gameLoss_Update()
{
	if (AEInputCheckTriggered(AEVK_Q))
	{
		next = GS_QUIT;
	}
}

void gameLoss_Draw()
{
	AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

	if (playerloss)
	{
		generic_draw(pMesh, player_defeat, 1.f, 800.f, 600.f, 0.f, 0.f);
	}

	else
		generic_draw(pMesh, nexus_defeat, 1.f, 800.f, 600.f, 0.f, 0.f);
}

void gameLoss_Free()
{
	AEGfxMeshFree(pMesh);
	delete gameUIManager;
}

void gameLoss_Unload()
{
	AEGfxTextureUnload(nexus_defeat);
	AEGfxTextureUnload(player_defeat);
}