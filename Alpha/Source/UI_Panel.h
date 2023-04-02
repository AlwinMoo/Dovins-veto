/******************************************************************************/
/*!
\file		UI_Button.h
\author 	Alonzo Nalpon(a.nalpon)
\par    	email: a.nalpon@digipen.edu
\date   	April 01, 2023
\brief		This file contains the definition of panel class.
			Panels provide a background for ui elements like buttons

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#ifndef UI_PANEL_H
#define UI_PANEL_H
#include "AEEngine.h"
#include "UI_Def.h"
namespace UI
{
	class UI_Panel {
	private:
		AEVec2 m_pos;	//!< position of panel
		AEVec2 m_scale;	//!< scale of panel
		UI_TEX m_tex;	//!< texture id of panel
	public:
		/// <summary>
		/// Ctor to create panel based on transformation and texture
		/// </summary>
		/// <param name="pos">position of panel</param>
		/// <param name="scale">scale of panel</param>
		/// <param name="tex">tex id of panel</param>
		UI_Panel(AEVec2 pos, AEVec2 scale, UI_TEX tex)
			: m_pos(pos), m_scale(scale), m_tex(tex) {}
		~UI_Panel() = default;

		/// <summary>
		/// Draw function to render panel
		/// </summary>
		void Draw()
		{
			DrawMesh(m_pos, m_scale, m_tex);
		}
	};
}
#endif