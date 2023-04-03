/******************************************************************************/
/*!
\file		UI_Def.cpp
\author 	Alonzo Nalpon
\author		Bevan Lim
\par    	email: a.nalpon@digipen.edu
\par    	email: bevanzhiwei.lim@digipen.edu
\date   	March 02, 2023
\brief		Alonzo (Primary 90%), Bevan (Secondary 10%)
			This file contains enumerations for textures, button types and meshes
			with helper functions to draw/print text on screen, init and unloading UI.
			All textures and mesh used by UI will be loaded and unloaded by UI_Def.
			Alonzo: functions, extern declarations, enumerations
			Bevan: additional enumerations for buttons/tex

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "UI_Def.h"
#include "Rendering.hpp"
#include "Menu.h"
namespace UI
{
	std::array<AEGfxTexture*, NUM_TEX> TextureList;
	std::array<AEGfxVertexList*, NUM_MESH> MeshList;
	void InitUI()
	{
		// LOAD ALL TEXTURES
		TextureList[TEX_BUTTON]			= AEGfxTextureLoad("Assets/SquareButton.png");
		TextureList[TEX_END_PHASE]		= AEGfxTextureLoad("Assets/Hourglass.png");
		TextureList[TEX_TOWER]			= AEGfxTextureLoad("Assets/Tower.png");
		TextureList[TEX_NEXUS]			= AEGfxTextureLoad("Assets/Nexus.png");
		TextureList[TEX_WALL]			= AEGfxTextureLoad("Assets/Wall.png");
		TextureList[TEX_NEXUS_PLACED]	= AEGfxTextureLoad("Assets/NexusPlaced.png");
		TextureList[TEX_ERASE]			= AEGfxTextureLoad("Assets/Eraser.png");
		TextureList[TEX_PLAYER]			= AEGfxTextureLoad("Assets/standing-still.png");
		TextureList[TEX_PLAYER_PLACED]	= AEGfxTextureLoad("Assets/player-gone.png");
		TextureList[TEX_SKILL_MENU]		= AEGfxTextureLoad("Assets/UpgradeIcon.png");
		TextureList[TEX_CLOSE]			= AEGfxTextureLoad("Assets/Close.png");
		TextureList[TEX_GUN_SKILL]		= AEGfxTextureLoad("Assets/GunSkill.png");
		TextureList[TEX_AOE_SKILL]		= AEGfxTextureLoad("Assets/AOESkill.png");
		TextureList[TEX_HEAL_SKILL]		= AEGfxTextureLoad("Assets/HealSkill.png");
		TextureList[TEX_READY]			= AEGfxTextureLoad("Assets/Ready.png");
		TextureList[TEX_BLUE_NODE]		= AEGfxTextureLoad("Assets/blueNode_Badea2.png");
		TextureList[TEX_YELLOW_NODE]	= AEGfxTextureLoad("Assets/yellowNode_Badea2.png");
		TextureList[TEX_GREEN_NODE]		= AEGfxTextureLoad("Assets/greenNode_Badea2.png");
		TextureList[TEX_UI_BUTTON]		= AEGfxTextureLoad("Assets/UIButton.png");
		TextureList[TEX_PANEL]			= AEGfxTextureLoad("Assets/UIPanel.png");
		// LOAD THAT ONE MESH WE ALL NEED
		MeshList[MESH_BOX] = basic_mesh();
	}

	void UnloadUI()
	{
		// CLEAR ALL TEXTURES
		for (AEGfxTexture* i : TextureList) {
			if (!i)
				continue;
			AEGfxTextureUnload(i);
		}
		// CLEAR ALL MESHES
		for (AEGfxVertexList* i : MeshList) {
			if (!i)
				continue;
			AEGfxMeshFree(i);
		}

	}

	void AEGfxPrint(s8 const* pStr, AEVec2 const& screenPosN, f32 scale, UI_Color const& color)
	{
		::AEGfxPrint(g_fontID, const_cast<s8*>(pStr), screenPosN.x, screenPosN.y, scale, color.r, color.g, color.b);
	}

	void AEGfxGetPrintSize(s8 const* pStr, AEVec2& textDimsN, f32 scale)
	{
		::AEGfxGetPrintSize(g_fontID, const_cast<s8*>(pStr), textDimsN.x, textDimsN.y, scale);
	}

	void DrawMesh(AEVec2 const& pos, AEVec2 const& meshScale, UI_TEX texID)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTintColor(1.f, 1.f, 1.f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.f);
		AEGfxTextureSet(TextureList[texID], 0, 0);

		// Perform transformations
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, meshScale.x, meshScale.y);

		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, 0);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, pos.x, pos.y);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);	// translate * rotate * scale
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m); // apply transform
		AEGfxMeshDraw(MeshList[MESH_BOX], AE_GFX_MDM_TRIANGLES);
	}

	void DrawMesh(AEVec2 const& pos, AEVec2 const& meshScale, UI_TEX texID, UI_Color const& color)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE); // stop other settings from leaking in
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetBlendColor(color.r, color.g, color.b, color.a);
		DrawMesh(pos, meshScale, texID);		// Draw mesh
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);	// Reset blend color
	}
}