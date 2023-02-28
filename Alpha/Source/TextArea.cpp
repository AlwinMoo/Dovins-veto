#include "TextArea.h"
#include <iostream>
#include <algorithm>
namespace UI{
	const s8 FONT_ID{ 1 };
	void TextArea::GetPrintSize(std::string const& str, f32& width, f32& height)
	{
		AEGfxGetPrintSize(FONT_ID, const_cast<s8*>(str.c_str()), m_scale, width, height);
	}

	u32 TextArea::ProcessLine(std::string const& line)
	{
		f32 width, height;
		GetPrintSize(line, width, height);
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

	std::string TextArea::SplitLine(std::string& line, u32 end)
	{
		std::string newStr(line, 0, end);
		line.erase(0, end);
		return newStr;
	}

	void TextArea::SetString(std::string& str)
	{
		u32 index;
		while (index = ProcessLine(str)) {

			lines.push_back(TextLineInfo{ SplitLine(str, index) , m_scale});
		}
		lines.push_back(TextLineInfo{ str, m_scale});
	}

	TextArea::TextArea() : TextArea(0,0)
	{
	}

	TextArea::TextArea(f32 w, f32 h, f32 scale) : m_boxWidthN(w), m_boxHeightN(h), m_scale(scale)
	{
		m_boxWidth	= AEGetWindowWidth()	* m_boxWidthN;
		m_boxHeight = AEGetWindowHeight()	* m_boxHeightN;
	}


	TextArea::TextArea(f32 w, f32 h, std::string str, f32 scale)  : TextArea(w,h, scale)
	{
		// automatically calculate and sort strings
		while (u32 index{ ProcessLine(str) }) 
		{
			lines.push_back(TextLineInfo{ SplitLine(str, index) , m_scale });
		}
		lines.push_back(TextLineInfo{ str, m_scale });
	}

	void TextArea::Draw(f32 screenX, f32 screenY, f32 r, f32 g, f32 b)
	{
		f32 screenHeight{ static_cast<f32>(AEGetWindowHeight()) };
		f32 screenWidth{ static_cast<f32>(AEGetWindowWidth()) };
		f32 offsetY{lines.at(0).Height()};

		// Automatically offset x based on screen bounds
		screenX = (screenX < -1.f ? -1.f : (screenX > 1.f - m_boxWidthN ? 1.f - m_boxWidthN : screenX));
		for (TextLineInfo const& i : lines) {
			//std::cout << i.GetString() << std::endl;
			AEGfxPrint(FONT_ID, const_cast<s8*>(i.GetString()), screenX, screenY - offsetY, m_scale, r, g, b);
			offsetY += i.Height();
		}
	}

	TextArea::~TextArea()
	{
		std::cout << "DESTROYED" << std::endl;
	}

}