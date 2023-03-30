#include "UI_Def.h"
#include "Rendering.hpp"

namespace UI
{
	std::array<AEGfxTexture*, NUM_TEX> TextureList;
	std::array<AEGfxVertexList*, NUM_MESH> MeshList;
	void InitUI()
	{
		TextureList[TEX_BUTTON]			= AEGfxTextureLoad("Assets/SquareButton.png");
		TextureList[TEX_END_PHASE]		= AEGfxTextureLoad("Assets/Hourglass.png");
		TextureList[TEX_TOWER]			= AEGfxTextureLoad("Assets/Tower.png");
		TextureList[TEX_NEXUS]			= AEGfxTextureLoad("Assets/Nexus.png");
		TextureList[TEX_WALL]			= AEGfxTextureLoad("Assets/Wall.png");
		TextureList[TEX_NEXUS_PLACED]	= AEGfxTextureLoad("Assets/NexusPlaced.png");
		TextureList[TEX_ERASE]			= AEGfxTextureLoad("Assets/Eraser.png");
		TextureList[TEX_PLAYER]			= AEGfxTextureLoad("Assets/PlayerTexture.png");
		TextureList[TEX_PLAYER_PLACED]	= AEGfxTextureLoad("Assets/PlayerPlaced.png");
		TextureList[TEX_SKILL_MENU]		= AEGfxTextureLoad("Assets/UpgradeIcon.png");
		TextureList[TEX_CLOSE]			= AEGfxTextureLoad("Assets/Close.png");
		TextureList[TEX_GUN_SKILL]		= AEGfxTextureLoad("Assets/GunSkill.png");
		TextureList[TEX_AOE_SKILL]		= AEGfxTextureLoad("Assets/AOESkill.png");
		TextureList[TEX_READY] = AEGfxTextureLoad("Assets/Ready.png");

		MeshList[MESH_BOX] = render::GenerateQuad();
	}

	void UnloadUI()
	{
		for (AEGfxTexture* i : TextureList) {
			if (!i)
				continue;
			AEGfxTextureUnload(i);
		}
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

	void AEGfxGetPrintSize(s8 const* pStr, AEVec2& screenPosN, f32 scale)
	{
		::AEGfxGetPrintSize(g_fontID, const_cast<s8*>(pStr), screenPosN.x, screenPosN.y, scale);
	}

	void DrawMesh(AEVec2 const& pos, AEVec2 const& meshScale, UI_TEX texID)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTintColor(1.f, 1.f, 1.f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.f);
		AEGfxTextureSet(TextureList[texID], 0, 0);

		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, meshScale.x, meshScale.y);

		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, PI);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, pos.x, pos.y);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(MeshList[MESH_BOX], AE_GFX_MDM_TRIANGLES);
	}
	void DrawMesh(AEVec2 const& pos, AEVec2 const& meshScale, UI_TEX texID, UI_Color const& color)
	{
		AEGfxSetBlendColor(color.r, color.g, color.b, color.a);
		DrawMesh(pos, meshScale, texID);
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
	}
}