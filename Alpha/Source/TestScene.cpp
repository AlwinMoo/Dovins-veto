#include "TestScene.h"
#include <iostream>
#include "Rendering.hpp"
#include "GameStateList.h"
#include "GameStateManager.h"

namespace
{
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE value)
	{
		for (auto it : go_list)
		{
			GameObject* go = (GameObject*)it;
			if (!go->active)
			{
				switch (go->type)
				{
				default:
					go->tex = planetTex;
				}
				go->active = true;
				++object_count;
				return go;
			}
		}
		GameObject* go{ new GameObject(value) };
		go_list.push_back(go);

		//CODE TO INITIALISE GO SPECIFIC VARIABLES

		return FetchGO(value);

	}


}

AEGfxTexture* texTest;
AEGfxVertexList* meshTest;
void TestScene_Load()
{
	planetTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	grassTex = AEGfxTextureLoad("Assets/GrassTile.png");

	
}

void CALLBACKTEST() {
	std::cout << "CALLED BACK" << std::endl;
}

void TestScene_Initialize()
{	
	// UI MANAGER
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenHeightY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		//auto meshTest = render::GenerateQuad();
		uiManager = new UI::UI_Manager();
		uiManager->SetWinDim(screenWidthX, screenHeightY);
	}
	srand(time(NULL));

	test_map = new game_map(10, 10, AEGetWindowWidth(), AEGetWindowHeight(), 4);

	for (int i = 0; i < test_map->width * test_map->height; i++)
	{
		GameObject* temp = FetchGO(GameObject::GO_TILE);
		temp->scale.x = test_map->GetTileSize();
		temp->scale.y = test_map->GetTileSize();
		temp->position = test_map->GetWorldPos(i);
		temp->tex = grassTex;
	}

	hoverStructure = FetchGO(GameObject::GO_HOVER_STRUCTURE);
	hoverStructure->alpha = 0.5f;
	hoverStructure->rotation = rand() % 360;
	hoverStructure->scale.x = test_map->GetTileSize();
	hoverStructure->scale.y = hoverStructure->scale.x;
	validPlacement = false;

	// UI MANAGER
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenWidthY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		const AEVec2 buttonPos{ screenWidthX / 4, screenWidthY / 4 };
		const AEVec2 buttonSize{ 50.f, 50.f };
		uiManager->CreateButton(buttonPos, buttonSize, UI::WHITE_BUTTON, CALLBACKTEST);
	}
}

void TestScene_Update()
{
	// Player Input
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEVec2 mouse_pos{};
	AEVec2Set(&mouse_pos, mouseX, mouseY);
	//float mouseYGrid = static_cast<int>(mouseY / test_map->get_tile_size()) * test_map->get_tile_size() + (test_map->get_tile_size() * 0.5);
	{
		AEVec2 invert_mouse = mouse_pos;
		invert_mouse.y = uiManager->m_winDim.y -mouse_pos.y;
		uiManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
	}
	mouse_pos = test_map->SnapCoordinates(mouse_pos);
	//float mouseYGrid = static_cast<int>(mouseY / test_map->GetTileSize()) * test_map->GetTileSize() + (test_map->GetTileSize() * 0.5);

	// Place Structure
	if (AEInputCheckTriggered(AEVK_LBUTTON) && validPlacement)
	{
		GameObject* test = FetchGO(GameObject::GO_PLANET);
		test->position = hoverStructure->position;
		test->rotation = hoverStructure->rotation;
		test->scale = hoverStructure->scale;
		hoverStructure->rotation = rand() % 360;

		test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(mouse_pos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
	}

	// Update position of hover structure
	if (hoverStructure->position.x != mouse_pos.x
		|| hoverStructure->position.y != mouse_pos.y)
	{
		hoverStructure->position = mouse_pos;
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
	}

	// Turn on/off hover if in/out the grid
	if (test_map->IsInGrid(mouse_pos, hoverStructure->gridScale.x, hoverStructure->gridScale.y))
	{
		hoverStructure->active = true;
		validPlacement = true;
	}
	else
	{
		hoverStructure->active = false;
		validPlacement = false;
	}

	// Change selected structure
	if (AEInputCheckTriggered(AEVK_1))
	{
		hoverStructure->gridScale = { 1, 1 };
		hoverStructure->scale = { test_map->GetTileSize(), test_map->GetTileSize() };
		hoverStructure->position = mouse_pos;
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
	}
	else if (AEInputCheckTriggered(AEVK_2))
	{
		hoverStructure->gridScale = { 2, 2 };
		hoverStructure->scale = { test_map->GetTileSize() * 2, test_map->GetTileSize() * 2 };
		hoverStructure->position = mouse_pos;
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
	}
	else if (AEInputCheckTriggered(AEVK_3))
	{
		hoverStructure->gridScale = { 3, 3 };
		hoverStructure->scale = { test_map->GetTileSize() * 3, test_map->GetTileSize() * 3 };
		hoverStructure->position = mouse_pos;
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
	}
	else if (AEInputCheckTriggered(AEVK_4))
	{
		//DOESN'T WORK
		hoverStructure->gridScale = { 2, 4 };
		hoverStructure->scale = { test_map->GetTileSize() * 2, test_map->GetTileSize() * 4 };
		hoverStructure->position = mouse_pos;
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
	}
	else if (AEInputCheckTriggered(AEVK_5))
	{
		//DOESN'T WORK
		hoverStructure->gridScale = { 5, 2 };
		hoverStructure->scale = { test_map->GetTileSize() * 5, test_map->GetTileSize() * 2 };
		hoverStructure->position = mouse_pos;
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
	}

	// Quit Game
	if (AEInputCheckTriggered(AEVK_Q))
	{
		next = GS_QUIT;
	}

	// GameObject Update
	for (GameObject* gameObj : go_list)
	{
		if (gameObj->active)
			gameObj->Update();
	}

	

	// GameObject Collision (GRID BASED)
	if (test_map->IsOccupied(test_map->WorldToIndex(mouse_pos), hoverStructure->gridScale.x, hoverStructure->gridScale.y))
	{
		hoverStructure->color.Set(1.0f, 0.2f, 0.2f);
		validPlacement = false;
	}
	else if (hoverStructure->active)
	{
		hoverStructure->color.Set(1.0f, 1.0f, 1.0f);
		validPlacement = true;
	}

	// GameObject Collision (NON-GRID BASED, SHOULD CHANGE)
	//bool hoverCollided = false;
	//for (GameObject* gameObj : go_list)
	//{
	//	if (!gameObj->active)
	//		continue;
	//
	//	if (gameObj != hoverStructure)
	//	{
	//		if (gameObj->type == GameObject::GO_TILE)
	//			continue;
	//		if (hoverStructure->position.x == gameObj->position.x &&
	//			hoverStructure->position.y == gameObj->position.y)
	//		{
	//			hoverStructure->color.Set(1.0f, 0.2f, 0.2f);
	//			hoverCollided = true;
	//			validPlacement = false;
	//			break;
	//		}
	//	}
	//}
	//
	//if (!hoverCollided && hoverStructure->active)
	//{
	//	hoverStructure->color.Set(1.0f, 1.0f, 1.0f);
	//	validPlacement = true;
	//}
}

void TestScene_Draw()
{
	s32 cursorX, cursorY;
	//s8 text[] = "TEST";
	AEInputGetCursorPosition(&cursorX, &cursorY);
	//f32 fcursorX, fcursorY;
	//
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	//
	//// IMPORTANT SNIPPET:
	///***********************************************************************/
	//f32 winX{ AEGfxGetWinMaxX() - AEGfxGetWinMinX() }, winY{ AEGfxGetWinMaxY() - AEGfxGetWinMinY() };
	////std::cout << winX << ", " <<  winY << std::endl;
	//f32 cursorXN{ cursorX / winX * 2 - 1.f }, cursorYN{ cursorY / winY * -2 + 1.f }; // NORMALIZED COORDINATES
	//std::cout << cursorXN << ", " << cursorYN << std::endl;
	///************************************************************************/
	//s8 const* testStr = "BUILD PHASE";
	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	////AEGfxPrint(m_fontId, (s8*)testStr, cursorXN, cursorYN, 2.f, 1.f, 0.f, 0.f);

	AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);
	for (GameObject* gameObj : go_list)
	{
		//Gameobjects Render
		if (gameObj->active)
			gameObj->Render();
	}

	// Render above
	if(hoverStructure->active)
		hoverStructure->Render();

	f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	f32 screenWidthY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	{
	 // THE WHERE'S MY CURSOR TEST?
	 AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	 AEGfxSetTintColor(1.f, 1.f, 1.f, 1.0f);
	 AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	 AEGfxSetTransparency(1.f);
	 AEGfxTextureSet(texTest, 0, 0);

	 AEMtx33 scale = { 0 };
	 AEMtx33Scale(&scale, 50.f, 50.f);

	 AEMtx33 rotate = { 0 };
	 AEMtx33Rot(&rotate, 0.f);

	 AEMtx33 translate = { 0 };
	 //std::cout << cursorX << ", " << cursorY << std::endl;
	 AEMtx33Trans(&translate, cursorX - screenWidthX /2, -cursorY + screenWidthY/2 - 50);

	 AEMtx33 transform = { 0 };
	 AEMtx33Concat(&transform, &rotate, &scale);
	 AEMtx33Concat(&transform, &translate, &transform);

	 AEGfxSetTransform(transform.m);
	 AEGfxMeshDraw(uiManager->m_mesh[0], AE_GFX_MDM_TRIANGLES);
	// Render UI
	uiManager->Draw();
	}
}

void TestScene_Free()
{
	delete uiManager;
}

void TestScene_Unload()
{

}