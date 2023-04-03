/******************************************************************************/
/*!
\file		UI_StatElement.cpp
\authors 	Alonzo Nalpon(a.nalpon)
\par    	email: a.nalpon@digipen.edu
\date   	March 02, 2023
\brief		This file contains the definitions of UI_StatInfo class.
			These elements can render text and/or mesh

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "UI_StatElement.h"

namespace UI {



void UI_StatElement::SetElementPos(AEVec2 const& pos)
{
	data.meshPos = pos;
}

void UI_StatElement::CalculatePositions()
	{
		AEVec2 textSize{};
		if (data.textArea)
		{
			textSize = { data.textArea->GetBoxWidthN(), data.textArea->GetBoxHeightN() };
			data.posN.x = data.posWS.x / AEGetWindowWidth() * 2.f - 1.f;
			data.posN.y = data.posWS.y / AEGetWindowHeight() * 2.f - 1.f;
		}

		// Offset the mesh to the right based on text
		data.meshPos.x = data.posWS.x + textSize.x * 0.5f * static_cast<f32>(AEGetWindowWidth());
	}

	void UI_StatElement::SetValue(double val)
	{
		data.value = val;
	}

	double UI_StatElement::GetValue() const
	{
		return data.value;
	}

	void UI_StatElement::SetColor(UI_Color const& color)
	{
		data.colorTint = color;
	}

	UI_StatElement::UI_StatElement(AEVec2 pos, UI_TextArea* text, AEVec2 const& meshDims) : data{}
	{
		data.textArea = text;
		data.meshScale = meshDims;
		data.posWS = pos;
	}

	void UI_StatElement::Draw()
	{
		// Render text if any
		if (data.textArea)
			data.textArea->Draw(data.posN, data.colorTint);

		// Render the mesh
		AEVec2 scale{ static_cast<float>(data.value) * data.meshScale.x, data.meshScale.y };
		UI::DrawMesh(data.meshPos, scale, data.m_tex, data.colorTint);
	}

}