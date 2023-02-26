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
		// Call this after setting position, scale or bounds
		void ConvertToWS(Button* newButton);
	public:
		std::array<AEGfxTexture*, NUM_TEX> m_textures;
		std::array<AEGfxVertexList*, NUM_MESH> m_mesh;
		std::vector<Button*> m_buttons;
		AEVec2 m_winDim;
		s8 m_fontID;

		
		void CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, TextArea* buttonText = nullptr, void(*callback)() = nullptr, TextArea* hoverText = nullptr);

		void Load();

		
		void Unload();
		void SetWinDim(f32 x, f32 y);
		void Update(AEVec2 mousePos, bool lClick);
		void Draw(f32 const& mouseXN = 0.f, f32 const& mouseYN = 0.f);

		UI_Manager();
		~UI_Manager();
	};
}
#endif

