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

	// UI MANAGER
	
}

void CALLBACKTEST() {
	std::cout << "CALLED BACK" << std::endl;
}

void TestScene_Initialize()
{
	meshTest = render::GenerateQuad();
	srand(time(NULL));

	test_map = new game_map(10, 10, AEGetWindowWidth(), AEGetWindowHeight(), true);

	for (int i = 0; i < test_map->width * test_map->height; i++)
	{
		GameObject* temp = FetchGO(GameObject::GO_TILE);
		temp->scale.x = test_map->get_tile_size();
		temp->scale.y = test_map->get_tile_size();
		temp->position = test_map->get_worldpos(i);
		temp->tex = grassTex;
	}

	hoverStructure = FetchGO(GameObject::GO_HOVER_STRUCTURE);
	hoverStructure->alpha = 0.5f;
	hoverStructure->rotation = rand() % 360;
	hoverStructure->scale.x = test_map->get_tile_size();
	hoverStructure->scale.y = hoverStructure->scale.x;
	validPlacement = false;

	// UI MANAGER
#ifdef UI_TEST
	{
		uiManager.Load();
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenWidthY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		const AEVec2 buttonPos{ screenWidthX / 4, screenWidthY / 4 };
		const AEVec2 buttonSize{ 50.f, 50.f };
		uiManager.CreateButton(buttonPos, buttonSize, UI::WHITE_BUTTON, UI::FONT_ROBOTO, CALLBACKTEST);
	}
#endif
}

void TestScene_Update()
{
	// Player Input
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEVec2 mouse_pos{};
	AEVec2Set(&mouse_pos, mouseX, mouseY);
	mouse_pos = test_map->snap_coordinates(mouse_pos);
	//float mouseYGrid = static_cast<int>(mouseY / test_map->get_tile_size()) * test_map->get_tile_size() + (test_map->get_tile_size() * 0.5);

	// Place Structure
	if (AEInputCheckTriggered(AEVK_LBUTTON) && validPlacement)
	{
		GameObject* test = FetchGO(GameObject::GO_PLANET);
		test->position = hoverStructure->position;
		//test->position.y = mouseYGrid;
		test->rotation = hoverStructure->rotation;
		hoverStructure->rotation = rand() % 360;
		test->scale.x = test_map->get_tile_size();
		test->scale.y = test->scale.x;
	}

	// Update position of hover structure
	if (hoverStructure->position.x != mouse_pos.x
		|| hoverStructure->position.y != mouse_pos.y)
	{
		hoverStructure->position = mouse_pos;
	}

	// Turn on/off hover if in/out the grid
	if (test_map->is_in_grid(mouse_pos))
	{
		hoverStructure->active = true;
		validPlacement = true;
	}
	else
	{
		hoverStructure->active = false;
		validPlacement = false;
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
		{
			if (gameObj->type == GameObject::GO_PLANET)
				gameObj->Update();
		}
	}

	// GameObject Collision (NON-GRID BASED, SHOULD CHANGE)
	bool hoverCollided = false;
	for (GameObject* gameObj : go_list)
	{
		if (gameObj != hoverStructure)
		{
			if (hoverStructure->position.x == gameObj->position.x &&
				hoverStructure->position.y == gameObj->position.y)
			{
				hoverStructure->color.Set(1.0f, 0.2f, 0.2f);
				hoverCollided = true;
				validPlacement = false;
				break;
			}
		}
	}
	if (!hoverCollided && hoverStructure->active)
	{
		hoverStructure->color.Set(1.0f, 1.0f, 1.0f);
		validPlacement = true;
	}
}

void TestScene_Draw()
{
	//s32 cursorX, cursorY;
	//s8 text[] = "TEST";
	//AEInputGetCursorPosition(&cursorX, &cursorY);
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

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	for (GameObject* gameObj : go_list)
	{
		//Gameobjects Render
		if (gameObj->active)
		{
			if (gameObj->type == GameObject::GO_TILE)
				gameObj->Render();
			if (gameObj->type == GameObject::GO_PLANET)
				gameObj->Render();
		}
	}

	// Render above
	if(hoverStructure->active)
		hoverStructure->Render();

	// UI TEST
	// AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// AEGfxSetTintColor(1.f, 1.f, 1.f, 1.0f);
	// AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	// AEGfxSetTransparency(1.f);
	// AEGfxTextureSet(texTest, 0, 0);

	// AEMtx33 scale = { 0 };
	// AEMtx33Scale(&scale, 1.f, 1.f);

	// AEMtx33 rotate = { 0 };
	// AEMtx33Rot(&rotate, 0.f);

	// AEMtx33 translate = { 0 };
	// AEMtx33Trans(&translate, AEGetWindowWidth() / 2.f, AEGetWindowHeight() / 2.f);

	// AEMtx33 transform = { 0 };
	// AEMtx33Concat(&transform, &rotate, &scale);
	// AEMtx33Concat(&transform, &translate, &transform);

	// AEGfxSetTransform(transform.m);
	// //AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

void TestScene_Free()
{
	uiManager.Unload();
}

void TestScene_Unload()
{

}