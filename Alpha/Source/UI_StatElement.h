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

		/// <summary>
		/// Set element position and calculate text/mesh pos
		/// </summary>
		/// <param name="pos">new pos</param>
		void SetElementPos(AEVec2 const& pos);

		/// <summary>
		/// Calculate positions of mesh offset from text
		/// </summary>
		void CalculatePositions();

		/// <summary>
		/// Set value of double (for bars)
		/// </summary>
		/// <param name="val"></param>
		void SetValue(double val);

		/// <summary>
		/// Obtains double value held by element
		/// </summary>
		/// <returns></returns>
		double GetValue()const;
		
		/// <summary>
		/// set tint color of mesh
		/// </summary>
		/// <param name="color"></param>
		void SetColor(UI_Color const& color);

		/// <summary>
		/// Draw the element. Draws text if any followed by mesh
		/// </summary>
		void Draw();
	};
}
#endif
