#include "TextArea.h"
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
		if (width < m_boxWidth)
			return 0;

		std::string lineCpy(line);

		do 
		{
			lineCpy.erase(lineCpy.end() - 1);
			GetPrintSize(lineCpy, width, height);
		} while (width >= m_boxWidth);
		return lineCpy.size() - 1;
	}

	std::string TextArea::SplitLine(std::string& line, u32 index)
	{
		std::string newStr(line, 0, index);
		line.erase(0, index);
		return newStr;
	}

	void TextArea::SetString(std::string& str)
	{
		u32 index;
		while (index = ProcessLine(str)) {
			lines.push_back(SplitLine(str, index));
		}
	}

	TextArea::TextArea() : TextArea(0,0)
	{
	}

	TextArea::TextArea(s32 w, s32 h) : m_boxWidth(w), m_boxHeight(h), m_scale(1.f)
	{

	}


	TextArea::TextArea(s32 w, s32 h, std::string const& str)  : TextArea(w,h)
	{
		// automatically calculate 
	}

	void TextArea::Draw()
	{

	}

	TextArea::~TextArea()
	{

	}

}