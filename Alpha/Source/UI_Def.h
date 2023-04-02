/******************************************************************************/
/*!
\file		UI_Def.h
\author 	Alonzo Nalpon -> functions, extern declarations, enumerations
\author		Bevan Lim -> additional enumerations for buttons/tex
\par    	email: a.nalpon@digipen.edu
\par    	email: bevanzhiwei.lim@digipen.edu
\date   	March 02, 2023
\brief		Alonzo (Primary 90%), Bevan (Secondary 10%)
			This file contains enumerations for textures, button types and meshes
			with helper functions to draw/print text on screen, init and unloading UI.
			All textures and mesh used by UI will be loaded and unloaded by UI_Def.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#ifndef UI_DEF_H
#define UI_DEF_H
#include "AEEngine.h"
#include <array>

// Types
extern s8 g_fontID; // Defined in main: the font used by all
namespace UI
{
	/// <summary>
	/// Enumerations for all button types.
	/// To be used when calling UI_Manager::CreateButton
	/// </summary>
	enum BUTTON_TYPE {
		WHITE_BUTTON,
		END_PHASE_BUTTON,
		BUILD_TOWER_BUTTON,
		BUILD_NEXUS_BUTTON,
		BUILD_WALL_BUTTON,
		BUILD_PLAYER_BUTTON,
		ERASE_BUTTON,
		SKILL_TREE_BUTTON,
		CLOSE_BUTTON,
		GUN_SKILL_BUTTON,
		AOE_SKILL_BUTTON,
		UTILITY_SKILL_BUTTON,
		HEAL_SKILL_BUTTON,
		CREDIT_BUTTON,
		RESTART_BUTTON,
		MENU_BUTTON,
		UI_MENU_BUTTON,
		NUM_BUTTONS
	};

	/// <summary>
	/// Enumerations for meshes used by ui buttons/ elements
	/// </summary>
	enum UI_MESH
	{
		MESH_BOX,
		NUM_MESH
	};

	/// <summary>
	/// Textures for all UI elements/buttons
	/// </summary>
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
		TEX_SKILL_MENU,
		TEX_CLOSE,
		// SKILL TEXTURES
		TEX_YELLOW_NODE,
		TEX_BLUE_NODE,
		TEX_GREEN_NODE,
		TEX_GUN_SKILL,
		TEX_AOE_SKILL,
		TEX_HEAL_SKILL,
		// Other UI elements
		TEX_READY,
		TEX_CREDIT,
		TEX_MENU,
		TEX_UI_BUTTON,
		TEX_RESTART,
		TEX_PANEL, // Panel for button background
		NUM_TEX
	};

	/// <summary>
	/// Types of UI stat elements
	/// </summary>
	enum UI_STAT_ELEMENT_TYPE
	{
		UI_STAT_ELEMENT_INT = 0,
		UI_STAT_ELEMENT_FLOAT,
		UI_STAT_ELEMENT_BAR,
		UI_STAT_TEXT,
		UI_STAT_ELEMENT_TOTAL
	};

	struct UI_Color { f32 r, g, b, a; }; //!< simple color struct to work with AE engine. In order: [r]ed, [g]reen, [b]lue, [a]lpha
}

// Functions
namespace UI
{
	/// <summary>
	/// Loads all required textures and meshes. Call once on app startup
	/// </summary>
	void InitUI();

	/// <summary>
	/// Unloads all loaded required textures amd meshes. Call once on app termination
	/// </summary>
	void UnloadUI();

	/// <summary>
	/// Alternative print function involving more compact parameters
	/// </summary>
	/// <param name="pStr">string to print</param>
	/// <param name="screenPosN">position of text with in range [-1, 1]</param>
	/// <param name="scale">scale of text</param>
	/// <param name="color">color of text</param>
	void AEGfxPrint(s8 const* pStr, AEVec2 const& screenPosN, f32 scale, UI_Color const& color);

	/// <summary>
	/// Alternative print size function involving more compact parameters
	/// </summary>
	/// <param name="pStr">string to measure</param>
	/// <param name="textDimsN">reference to be assigned width and height</param>
	/// <param name="scale">scale of text</param>
	void AEGfxGetPrintSize(s8 const* pStr, AEVec2& textDimsN, f32 scale);

	/// <summary>
	/// Draw a mesh based on pos, scale and texture onto a quad
	/// </summary>
	/// <param name="pos">position of mesh</param>
	/// <param name="meshScale">scale of mesh</param>
	/// <param name="texID">texture ID of mesh</param>
	void DrawMesh(AEVec2 const& pos, AEVec2 const& meshScale, UI_TEX texID);

	/// <summary>
	/// Draws a mesh with a tint color
	/// </summary>
	/// <param name="pos">position of mesh</param>
	/// <param name="meshScale">scale of mesh</param>
	/// <param name="texID">texture ID of mesh</param>
	/// <param name="color">color of the mesh to be tinted</param>
	void DrawMesh(AEVec2 const& pos, AEVec2 const& meshScale, UI_TEX texID, UI_Color const& color);
}

// Globals
namespace UI
{
	extern std::array<AEGfxTexture*, NUM_TEX> TextureList; // !< array of textures to be loaded on InitUI()
	extern std::array<AEGfxVertexList*, NUM_MESH> MeshList; //!< array of meshlists to be loaded on InitUI()
}
#endif
