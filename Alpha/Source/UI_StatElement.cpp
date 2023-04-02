/******************************************************************************/
/*!
\file		pathfinder.cpp
\author		Alwin Moo
\par        email: moo.g\@digipen.edu
\date       April 02, 2023
\brief		Alwin (Primary 100%)

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "UI_StatElement.h"

namespace UI {

namespace	// Anonymous namespace
{
	// All variations of render functions go here
	//UI_STAT_TEXT,
	void DrawElement(UI_StatInfo const& info)
	{
		// Render text if any
		if (info.textArea)
			info.textArea->Draw(info.posN, info.colorTint);

		// Render the mesh
		AEVec2 scale{ static_cast<float>(info.value) * info.meshScale.x, info.meshScale.y };
		UI::DrawMesh(info.meshPos, scale, info.m_tex, info.colorTint);
	}

	
	//UI_STAT_ELEMENT_INT = 0,
	//UI_STAT_ELEMENT_FLOAT,
	//UI_STAT_ELEMENT_TOTAL
	void CalculateIntPos(UI_StatInfo& info) 
	{
		AEVec2 textSize{};
		if (info.textArea)
		{
			textSize = { info.textArea->GetBoxWidthN(), info.textArea->GetBoxHeightN() };
			info.textArea->Draw(info.posN);
		}

		// Offset the mesh to the right based on text
		info.meshPos.x = info.posWS.x + textSize.x * 0.5f * static_cast<f32>(AEGetWindowWidth());
	}
} // Anonymous namespace end


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

	UI_StatElement::UI_StatElement(AEVec2 pos, UI_TextArea* text, AEVec2 const& meshDims) : m_drawFuncArr{}, data{}
	{
		data.textArea = text;
		data.meshScale = meshDims;
		data.posWS = pos;
	}

	void UI_StatElement::Update(double dt)
	{
		
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