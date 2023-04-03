/******************************************************************************/
/*!
\file		UI_Manager.h
\author 	Alonzo Nalpon (a.nalpon)
\par    	email: a.nalpon@digipen.edu
\date   	January 26, 2023
\brief		This file contains definition for the UI Manager class

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

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
		/// <summary>
		/// calculates world space positions on button
		/// </summary>
		/// <param name="newButton">button to modify</param>
		void ConvertToWS(UI_Button* newButton);
	public:
		/// <summary>
		/// Creates and return UIstatelements to be manually rendered by user
		/// </summary>
		/// <param name="pos">position of mesh</param>
		/// <param name="meshDim">dimensions of mesh</param>
		/// <param name="text">text, if any</param>
		/// <returns>pointer to the element created</returns>
		static UI_StatElement* GenerateUIStat(AEVec2 pos, AEVec2 meshDim, UI_TextArea* text = nullptr);

		std::vector<UI_Panel*> m_panels;				//!< UI panels
		std::vector<UI_Button*> m_buttons;				//!< UI buttons
		std::vector<UI_StatElement*> m_statElements;	//!< UI elements
		AEVec2 m_winDim; // !< dimensions of screen

		/// <summary>
		/// Creates a UI panel that renders behind buttons and elements
		/// </summary>
		/// <param name="pos">position of panel</param>
		/// <param name="scale">scale of panel</param>
		/// <param name="tex">texture of panel</param>
		void CreatePanel(AEVec2 pos, AEVec2 scale, UI_TEX tex);

		/// <summary>
		/// Creates a UI button that can have callback, text and/or hover text
		/// </summary>
		/// <param name="pos">position of button</param>
		/// <param name="size">scale of button</param>
		/// <param name="type">type of button</param>
		/// <param name="buttonText">text on button</param>
		/// <param name="callback">callback for button</param>
		/// <param name="hoverText">hover text on button</param>
		/// <param name="enabled">initial enable state of button</param>
		/// <returns>pointer to button created</returns>
		UI_Button* CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, UI_TextArea* buttonText = nullptr, 
			void(*callback)(UI_Button*) = nullptr, UI_TextArea* hoverText = nullptr, bool enabled = true);

		/// <summary>
		/// Creates a UI stat element that renders after buttons.
		/// stat elements can have either text or mesh or both
		/// </summary>
		/// <param name="pos">position of element</param>
		/// <param name="meshDim">dimensions of mesh</param>
		/// <param name="text">text if any</param>
		/// <returns>pointer to element created</returns>
		UI_StatElement* CreateUIStat(AEVec2 pos, AEVec2 meshDim, UI_TextArea* text = nullptr);

		/// <summary>
		/// Renders statelements created by static function, generate UI stat.
		/// </summary>
		/// <param name="element">element to be rendered</param>
		void RenderUIElement(UI_StatElement& element);

		/// <summary>
		/// Unloads all created buttons/elements/panels under manager
		/// </summary>
		void Unload();

		/// <summary>
		/// Sets window dimensions recorded by manager
		/// </summary>
		/// <param name="x">x dimensions</param>
		/// <param name="y">y dimensions</param>
		void SetWinDim(f32 x, f32 y);

		/// <summary>
		/// Update all UI elements under the manager based on mouse and click state
		/// </summary>
		/// <param name="mousePos">mouse position on screen</param>
		/// <param name="lClick">click state on left mouse</param>
		void Update(AEVec2 mousePos, bool lClick);

		/// <summary>
		/// Draws buttons and associated objects. Additional behaviours rely on mouse pos
		/// </summary>
		/// <param name="mouseX">screenspace mouse coordinates [-width/2, width/2]</param>
		/// <param name="mouseY">screenspace mouse coordinates [-height/2, height/2]</param>
		void Draw(s32 mouseX = 0, s32 mouseY = 0);

		UI_Manager() = default;

		/// <summary>
		/// Dtor calls Unload();
		/// </summary>
		~UI_Manager();
	};
}
#endif

