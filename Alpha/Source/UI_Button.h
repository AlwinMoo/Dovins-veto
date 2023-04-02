/******************************************************************************/
/*!
\file		UI_Button.h
\author 	Alonzo Nalpon 95%, Alvin Yeo 5%
\par    	email: a.nalpon@digipen.edu
\date   	April 01, 2023
\brief		This file contains the definition of button class
			to be used with ui manager

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include "AEEngine.h"
#include "UI_TextArea.h"
#include "UI_Def.h"
namespace UI 
{
	struct UI_Button
	{
		UI_TEX texID;	//!< texture ID of the button 
		UI_MESH meshID; //!< mesh ID of the button
		
		AEVec2 posN;	//!< normalized pos for text
		AEVec2 pos;		//!< non-world space pos (origin is bottom left)
		AEVec2 posWS;	//!< world space pos

		AEVec2 min, max;		//!< Boundaries for screen space
		AEVec2 minWS, maxWS;	//!< World space boundaries for rendering
		AEVec2 scale;			//!< scale of button

		UI_TextArea* buttonText; //!< pointer to text of button
		void(*callback)(UI_Button*); //!< callback for button
		UI_TextArea* hoverText;		// !< text when button is hovered on
		bool bHovering;				// !< flag to indicate if button is hovered on
		bool bEnable;				// !< bool to indicate if button is active (inactive cannot be pressed and is darker
		bool bBought;				// !< ALVIN: bool to indicate if button remains pressed (second disable bool with different color)
		
		/// <summary>
		/// Calculate normalized positions for button text
		/// </summary>
		/// <param name="screenDims">dimensions of screen</param>
		void CalculatePosN(AEVec2 screenDims);
		UI_Button() = default;
		~UI_Button() = default;
	};
}
#endif
