/******************************************************************************/
/*!
\file		UI_StatElement.h
\authors 	Alonzo Nalpon(a.nalpon)
\par    	email: a.nalpon@digipen.edu
\date   	March 02, 2023
\brief		This file contains the definition of UI_StatInfo class.
			These elements can render text and/or mesh

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef UI_STAT_ELEMENT_H
#define UI_STAT_ELEMENT_H
#include "UI_Def.h"
#include "UI_TextArea.h"
#include <array>
#include <string>
namespace UI {

	struct UI_StatInfo 
	{
		AEVec2	posN;			//!< normalized pos for text
		AEVec2	posWS;			//!< world space pos of object
		AEVec2	meshPos;		//!< world space pos of mesh start
		AEVec2	meshScale;		//!< mesh scale (unmanipulated)
		UI_TextArea* textArea;	//!< text area
		
		double value;			//!< rendering bars

		UI_Color colorTint;		//!< color (including alpha) of image

		// textures
		UI_TEX m_tex{TEX_NEXUS};
	};


	// Prints out image followed by text
	class UI_StatElement
	{
		UI_StatInfo data;
	public:
		UI_StatElement(AEVec2 pos, UI_TextArea* text = nullptr, AEVec2 const& meshDims = {});
		~UI_StatElement() = default;
		UI_StatElement& operator=(const UI_StatElement&) = delete;
		void SetElementPos(AEVec2 const& pos);		//!< Set element position and calculate text/mesh pos
		void CalculatePositions();					//!< Calculate positions of mesh offset from text
		void SetValue(double val);					//!< Set value of double (for bars)
		double GetValue()const;
		void SetColor(UI_Color const& color);

		void Update(double dt);
		void Draw();
	};
}
#endif
