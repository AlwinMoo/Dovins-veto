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
			textSize = { info.textArea->GetBoxWidthN(), info.textArea->GetBoxHeightN() };

		// Offset the mesh to the right based on text
		info.meshPos.x = info.posWS.x + textSize.x * 0.5f * static_cast<f32>(AEGetWindowWidth());

	}
} // Anonymous namespace end


void UI_StatElement::SetElementPos(AEVec2 const& pos)
{
}

void UI_StatElement::CalculatePositions()
	{
		// TODO: calculate width and height of element (use text dims)
		CalculateIntPos(data);
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

	UI_StatElement::UI_StatElement(UI_TextArea* text, AEVec2 const& meshDims) : m_drawFuncArr{}, data{}
	{
		data.textArea = text;
		data.meshScale = meshDims;
	}

	void UI_StatElement::Update(double dt)
	{
		
	}

	void UI_StatElement::Draw()
	{
		DrawElement(data);
	}

}