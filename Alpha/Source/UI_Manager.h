#ifndef UI_MANAGER_H
#define UI_MANAGER_H
#include <array>
#include "AEEngine.h"
#include <vector>
#include "UI_Def.h"
#include "UI_Button.h"
#include "UI_StatElement.h"
#include "UI_Panel.h"
namespace UI 
{
	class UI_Manager
	{
		// Call this after setting position, scale or bounds
		void ConvertToWS(UI_Button* newButton);
	public:
		// creating ui stat without using ui manager: manual rendering
		static UI_StatElement* GenerateUIStat(AEVec2 pos, AEVec2 meshDim, UI_TextArea* text = nullptr);

		std::vector<UI_Panel*> m_panels;			//!< UI panels
		std::vector<UI_Button*> m_buttons;				//!< UI buttons
		std::vector<UI_StatElement*> m_statElements;	//!< UI elements
		AEVec2 m_winDim;
		s8 m_fontID;

		void CreatePanel(AEVec2 pos, AEVec2 scale, UI_TEX tex);
		UI_Button* CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, UI_TextArea* buttonText = nullptr, 
			void(*callback)(UI_Button*) = nullptr, UI_TextArea* hoverText = nullptr, bool enabled = true);
		UI_StatElement* CreateUIStat(AEVec2 pos, AEVec2 meshDim, UI_TextArea* text = nullptr);
		void RenderUIElement(UI_StatElement& element);
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

