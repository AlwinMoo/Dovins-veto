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
		UI::DrawMesh(info.meshPos, scale, info.m_tex);
	}

	
	//UI_STAT_ELEMENT_INT = 0,
	//UI_STAT_ELEMENT_FLOAT,
	//UI_STAT_ELEMENT_TOTAL
	void CalculateIntPos(UI_StatInfo& info) 
	{
		AEVec2 textSize{ info.textArea->GetBoxWidthN(), info.textArea->GetBoxHeightN() };
		info.meshPos.x = info.posWS.x + textSize.x * 0.5f * static_cast<f32>(AEGetWindowWidth());

	}
} // Anonymous namespace end


	void UI_StatElement::CalculatePositions()
	{
		// TODO: calculate width and height of element (use text dims)
		CalculateIntPos(data);
	}

	UI_StatElement::UI_StatElement(UI_TextArea* text) : m_drawFuncArr{}, data{}
	{
		data.textArea = text;
	}

	void UI_StatElement::Update(double dt)
	{
		
	}

	void UI_StatElement::Draw()
	{
		DrawElement(data);
	}

}