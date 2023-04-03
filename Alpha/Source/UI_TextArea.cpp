/******************************************************************************/
/*!
\file		UI_TextArea.cpp
\author		Alonzo Nalpon
\par        email: a.nalpon@digipen.edu
\date       January 28, 2023
\brief		Alonzo (Primary 100%)
			Contains definitions for a text box which includes text wrapping

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "UI_TextArea.h"
#include <iostream>
#include <algorithm>
namespace UI{
	const s8 FONT_ID{ 1 };

	namespace 
	{
		bool is_whitespace(char ch) {
			return ch == ' ';
		}
	}

	void UI_TextArea::GetPrintSize(std::string const& str, f32& width, f32& height)
	{
		::AEGfxGetPrintSize(FONT_ID, const_cast<s8*>(str.c_str()), m_scale, width, height);
	}

	u32 UI_TextArea::ProcessLine(std::string const& line)
	{
		f32 width, height;
		GetPrintSize(line, width, height);

		// Add height of this line first to total box height
		m_boxHeight += height * static_cast<f32>(AEGfxGetWinMaxY() - AEGfxGetWinMinY());

		// Check if the entire string fits in line
		if (width < m_boxWidthN)
			return 0;

		std::string lineCpy(line);
		std::string::const_reverse_iterator rIt{ line.rbegin() };
		
		bool isWord{};
		// Find suitable substring that fits width
		// If word is longer than actual width limit, string is cut within word
		do
		{
			lineCpy.erase(lineCpy.end() - 1);
			++rIt; // Decrease the reference iterator to index to cut
			if (!is_whitespace(*rIt))
				isWord = true;
			else
				isWord = false;
			GetPrintSize(lineCpy, width, height);
		} while (width >= m_boxWidthN);

		// Now iterate down to a whitespace if within a word
		if (isWord)
			while (isWord && rIt != line.rend())
			{
				isWord = is_whitespace(*rIt) ? false : true;
				++rIt;
			}
		if (int id = static_cast<int>(line.rend() - rIt))
			return static_cast<u32>(id);

		return static_cast<u32>(lineCpy.size());
	}

	std::string UI_TextArea::SplitLine(std::string& line, u32 end)
	{
		std::string newStr(line, 0, end);
		line.erase(0, end);
		return newStr;
	}

	UI_TextArea::UI_TextArea(f32 w, f32 h, UI::UI_Color color, f32 scale) : 
		m_boxWidthN(w), m_boxHeightN(h), m_boxHeight(0.f), m_color(color), m_scale(scale)
	{
		m_boxWidth = AEGetWindowWidth() * m_boxWidthN; // Width is determined before splitting lines
	}


	UI_TextArea::UI_TextArea(f32 w, f32 h, std::string str, UI::UI_Color color, f32 scale)  : UI_TextArea(w,h,color,scale)
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

	void UI_TextArea::Draw(AEVec2 screenPosN) const
	{
		Draw(screenPosN, m_color);
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
}