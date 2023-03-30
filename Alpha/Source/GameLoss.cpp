#include "GameLoss.h"

namespace
{
	AEGfxVertexList* pMesh;
	AEGfxTexture* player_defeat;
	AEGfxTexture* nexus_defeat;
	AEGfxTexture* Message;
}

void gameLoss_Load()
{
	player_defeat = AEGfxTextureLoad("Assets/playerLoss.png");
	nexus_defeat = AEGfxTextureLoad("Assets/nexusLoss.png");
}

void gameLoss_Initialize()
{
	pMesh = basic_mesh();
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
}

void gameLoss_Unload()
{
	AEGfxTextureUnload(nexus_defeat);
	AEGfxTextureUnload(player_defeat);
}