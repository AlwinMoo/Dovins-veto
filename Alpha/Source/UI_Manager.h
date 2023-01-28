#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <array>
#include "AEEngine.h"
#include <vector>
#include "Button.h"
namespace UI 
{
	enum BUTTON_TYPE {
		WHITE_BUTTON,
	};
	enum UI_MESH 
	{
		MESH_BOX,
		NUM_MESH
	};
	enum UI_TEX 
	{
		TEX_BUTTON,
		NUM_TEX
	};
	enum UI_FONT {
		FONT_ROBOTO,
		NUM_FONTS
	};
	class UI_Manager
	{
	public:
		std::array<AEGfxTexture*, NUM_TEX> m_textures;
		std::array<AEGfxVertexList*, NUM_MESH> m_mesh;
		std::array<s8*, NUM_FONTS> m_fonts;
		std::vector<Button*> m_buttons;

		void CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, s8 fontID = 0, void(*callback)(void*) = nullptr);

		void LoadFont(char const* path);
		void Unload();

		UI_Manager();
		~UI_Manager();
	};
}
#endif

