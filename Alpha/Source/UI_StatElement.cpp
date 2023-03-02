#include "UI_StatElement.h"

namespace UI {

namespace	// Anonymous namespace
{
	// All variations of render functions go here
	//UI_STAT_TEXT,
	void Draw(f32 xN, f32 yN, UI_Color const& color, std::string)
	{
		//AEGfxPrint
	}
	//UI_STAT_ELEMENT_INT = 0,
	//UI_STAT_ELEMENT_FLOAT,
	//UI_STAT_ELEMENT_TOTAL
}
void UI_StatElement::CalculatePositions()
{
	// TODO: calculate width and height of element (use text dims)
}
// Anonymous namespace end
	UI_StatElement::UI_StatElement() : m_drawFuncArr{}, m_value{}, m_WSPos{}, m_textNPosStart{}
	{

	}

	void UI_StatElement::Update(double dt)
	{
		if (m_bTimed)
		{

		}
	}

	void UI_StatElement::Draw()
	{
		// Draw image (if any)
		if (m_tex)
		{

		}
		// Draw the rest
		m_drawFuncArr[m_type]();
	}

}