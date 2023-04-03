/******************************************************************************/
/*!
\file		UI_TextLineInfo.cpp
\author		Alonzo Nalpon
\par        email: a.nalpon@digipen.edu
\date       February 26, 2023
\brief		Alonzo (Primary 100%)
Contains definitions to get information about the text UI

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "UI_TextLineInfo.h"
namespace UI
{
	UI_TextLineInfo::UI_TextLineInfo(std::string const& str, f32 scale) : line(str)
	{
		AEGfxGetPrintSize(1, const_cast<s8*>(str.c_str()), scale, m_width, m_height);
	}

	f32 UI_TextLineInfo::Height()const
	{
		return m_height;
	}

	f32 UI_TextLineInfo::Width()const
	{
		return m_width;
	}
	char const* UI_TextLineInfo::GetString()const
	{
		return line.c_str();
	}
}
