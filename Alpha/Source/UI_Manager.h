#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <array>
#include "AEEngine.h"
#include <vector>
#include "UI_Types.h"
#include "Button.h"
namespace UI 
{
	class UI_Manager
	{
	public:
		std::array<AEGfxTexture*, NUM_TEX> m_textures;
		std::array<AEGfxVertexList*, NUM_MESH> m_mesh;
		std::array<s8, NUM_FONTS> m_fonts;
		std::vector<Button*> m_buttons;

		void CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, UI_FONT fontID, void(*callback)() = nullptr);

		void Load();

		// TODO: UNLOAD
		void Unload();

		void Update(AEVec2 mousePos, bool lClick);
		void Draw();
		UI_Manager();
		~UI_Manager();
	};
}
#endif

