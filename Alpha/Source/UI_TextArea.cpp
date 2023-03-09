#include "UI_TextArea.h"
#include <iostream>
#include <algorithm>
namespace UI{
	const s8 FONT_ID{ 1 };
	// TODO: STORE COLOR IN TEXTAREA
	void UI_TextArea::GetPrintSize(std::string const& str, f32& width, f32& height)
	{
		::AEGfxGetPrintSize(FONT_ID, const_cast<s8*>(str.c_str()), m_scale, width, height);
	}

	u32 UI_TextArea::ProcessLine(std::string const& line)
	{
		f32 width, height;
		GetPrintSize(line, width, height);
		m_boxHeight += height * static_cast<f32>(AEGfxGetWinMaxY() - AEGfxGetWinMinY());
		if (width < m_boxWidthN)
			return 0;

		std::string lineCpy(line);

		do 
		{
			lineCpy.erase(lineCpy.end() - 1);
			GetPrintSize(lineCpy, width, height);
		} while (width >= m_boxWidthN);
		return lineCpy.size();
	}

	std::string UI_TextArea::SplitLine(std::string& line, u32 end)
	{
		std::string newStr(line, 0, end);
		line.erase(0, end);
		return newStr;
	}

	//!< NOT BEING USED AT THE MOMENT>  INCOMPLETE
	void UI_TextArea::SetString(std::string& str)
	{
		u32 index;
		while (index = ProcessLine(str)) {

			lines.push_back(UI_TextLineInfo{ SplitLine(str, index) , m_scale});
		}
		lines.push_back(UI_TextLineInfo{ str, m_scale});
	}

	UI_TextArea::UI_TextArea() : UI_TextArea(0,0)
	{
	}

	UI_TextArea::UI_TextArea(f32 w, f32 h, f32 scale) : m_boxWidthN(w), m_boxHeightN(h), m_boxHeight(0.f), m_scale(scale)
	{
		m_boxWidth = AEGetWindowWidth() * m_boxWidthN; // Width is determined before splitting lines
		//m_boxHeight = AEGetWindowHeight() * m_boxHeightN;
	}


	UI_TextArea::UI_TextArea(f32 w, f32 h, std::string str, f32 scale)  : UI_TextArea(w,h, scale)
	{
		// automatically calculate and sort strings

		//m_boxWidth = AEGetWindowWidth() * m_boxWidthN;
		//m_boxHeight = AEGetWindowHeight() * m_boxHeightN;
		while (u32 index{ ProcessLine(str) }) 
		{
			lines.push_back(UI_TextLineInfo{ SplitLine(str, index) , m_scale });
		}
		lines.push_back(UI_TextLineInfo{ str, m_scale });

	}

	void UI_TextArea::Draw(f32 screenX, f32 screenY, f32 r, f32 g, f32 b)const
	{
		f32 screenHeight{ static_cast<f32>(AEGetWindowHeight()) };
		f32 screenWidth{ static_cast<f32>(AEGetWindowWidth()) };
		f32 offsetY{lines.at(0).Height()};

		// Automatically offset x based on screen bounds
		screenX = (screenX < -1.f ? -1.f : (screenX > 1.f - m_boxWidthN ? 1.f - m_boxWidthN : screenX));
		for (UI_TextLineInfo const& i : lines) {
			::AEGfxPrint(FONT_ID, const_cast<s8*>(i.GetString()), screenX, screenY - offsetY, m_scale, r, g, b);
			offsetY += i.Height();
		}
	}

	// Shorter way to call draw
	void UI_TextArea::Draw(AEVec2 screenPosN, UI_Color color) const
	{
		Draw(screenPosN.x, screenPosN.y, color.r, color.g, color.b);
	}

	f32 UI_TextArea::GetBoxWidth()
	{
		return m_boxWidth;
	}

	f32 UI_TextArea::GetBoxHeight()
	{
		return m_boxHeight;
	}

	f32 UI_TextArea::GetBoxWidthN()
	{
		return m_boxWidthN;
	}

	f32 UI_TextArea::GetBoxHeightN()
	{
		return m_boxHeightN;
	}

	UI_TextArea::~UI_TextArea()
	{
		std::cout << "DESTROYED" << std::endl;
	}

}