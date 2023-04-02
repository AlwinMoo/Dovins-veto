/******************************************************************************/
/*!
\file		UI_TextLineInfo.h
\author		Alonzo Nalpon
\par        email: a.nalpon@digipen.edu
\date       February 26, 2023
\brief		Alonzo (Primary 100%)
Contains declarations to get information about the text UI

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/

#ifndef UI_TEXT_LINE_INFO_H
#define UI_TEXT_LINE_INFO_H
#include <string>
#include "AEEngine.h"
namespace UI
{
	class UI_TextLineInfo
	{
	public:
		UI_TextLineInfo(std::string const& str, f32 scale);
		~UI_TextLineInfo();
		f32 Height()const;
		f32 Width()const;
		char const* GetString()const;
	private:
		f32 m_height, m_width;
		std::string line;
	};
}
#endif
