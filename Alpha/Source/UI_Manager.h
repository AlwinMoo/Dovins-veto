#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <array>
#include "AEEngine.h"
#include <vector>
#include "UI_Types.h"
#include "UI_Button.h"
namespace UI 
{
	class UI_Manager
	{
		// Call this after setting position, scale or bounds
		void ConvertToWS(UI_Button* newButton);
	public:
		std::array<AEGfxTexture*, NUM_TEX> m_textures;
		std::array<AEGfxVertexList*, NUM_MESH> m_mesh;
		std::vector<UI_Button*> m_buttons;
		AEVec2 m_winDim;
		s8 m_fontID;

		
		UI_Button* CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, UI_TextArea* buttonText = nullptr, void(*callback)() = nullptr, UI_TextArea* hoverText = nullptr);

		void Load();

		
		void Unload();
		void SetWinDim(f32 x, f32 y);
		void Update(AEVec2 mousePos, bool lClick);
		/// <summary>
		/// Draws buttons and associated objects. Additional behaviours rely on mouse pos
		/// </summary>
		/// <param name="mouseX">screenspace mouse coordinates [-width/2, width/2]</param>
		/// <param name="mouseY">screenspace mouse coordinates [-height/2, height/2]</param>
		void Draw(s32 mouseX = 0, s32 mouseY = 0);

		UI_Manager();
		~UI_Manager();
	};
}
#endif

