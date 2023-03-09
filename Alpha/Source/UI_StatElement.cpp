#include "UI_StatElement.h"

namespace UI {

namespace	// Anonymous namespace
{
	// All variations of render functions go here
	//UI_STAT_TEXT,
	void DrawElement(UI_StatInfo const& info)
	{
		AEGfxPrint(info.str.c_str(), info.xN, info.yN, info.m_scale, info.colorFont);



		
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
	UI_StatElement::UI_StatElement() : m_drawFuncArr{}, data{}
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
		DrawElement(data);
	}

}