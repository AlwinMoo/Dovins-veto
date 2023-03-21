#ifndef UI_DEF_H
#define UI_DEF_H
#include "AEEngine.h"
#include <array>
extern s8 g_fontID;

// Types
namespace UI
{
	enum BUTTON_TYPE {
		WHITE_BUTTON,
		END_PHASE_BUTTON,
		BUILD_TOWER_BUTTON,
		BUILD_NEXUS_BUTTON,
		BUILD_WALL_BUTTON,
		BUILD_PLAYER_BUTTON,
		ERASE_BUTTON,
		NUM_BUTTONS
	};

	enum UI_MESH
	{
		MESH_BOX,
		NUM_MESH
	};

	enum UI_TEX
	{
		TEX_BUTTON,
		TEX_END_PHASE,
		TEX_NEXUS,
		TEX_NEXUS_PLACED,
		TEX_TOWER,
		TEX_WALL,
		TEX_ERASE,
		TEX_PLAYER,
		TEX_PLAYER_PLACED,
		NUM_TEX
	};

	enum UI_STAT_ELEMENT_TYPE
	{
		UI_STAT_ELEMENT_INT = 0,
		UI_STAT_ELEMENT_FLOAT,
		UI_STAT_ELEMENT_BAR,
		UI_STAT_TEXT,
		UI_STAT_ELEMENT_TOTAL
	};

	enum UI_TYPE
	{
		UI_TYPE_GAME,
		UI_TYPE_SKILL,
		NUM_UI_TYPE
	};

	struct UI_Color { f32 r, g, b, a; }; //!< simple color struct to work with AE engine. In order: [r]ed, [g]reen, [b]lue, [a]lpha
}

// Functions
namespace UI
{
	void InitUI();

	void UnloadUI();

	void AEGfxPrint(s8 const* pStr, AEVec2 const& screenPosN, f32 scale, UI_Color const& color);

	void AEGfxGetPrintSize(s8 const* pStr, AEVec2& screenPosN, f32 scale);

	void DrawMesh(AEVec2 const& pos, AEVec2 const& meshScale, UI_TEX texID);

	void DrawMesh(AEVec2 const& pos, AEVec2 const& meshScale, UI_TEX texID, UI_Color const& color);
}

// Globals
namespace UI
{
	extern std::array<AEGfxTexture*, NUM_TEX> TextureList;
	extern std::array<AEGfxVertexList*, NUM_MESH> MeshList;
}
#endif
