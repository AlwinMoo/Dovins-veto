/******************************************************************************/
/*!
\file		UI_TextLineInfo.h
\author		Alonzo Nalpon
\par        email: a.nalpon@digipen.edu
\date       February 26, 2023
\brief		Alonzo (Primary 100%)
Contains definition of UI_TextLineInfo class.

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
	/// <summary>
	/// Stores information of text meshes
	/// </summary>
	class UI_TextLineInfo
	{
	public:
		/// <summary>
		/// Creates a text line info object based on a string and scale.
		/// Also calculates and stores width and height of text based
		/// on normalized screen scale
		/// </summary>
		/// <param name="str">string for text</param>
		/// <param name="scale">scale of text</param>
		UI_TextLineInfo(std::string const& str, f32 scale);
		~UI_TextLineInfo() = default;

		/// <summary>
		/// Gets the height of text line based on normalized screen scale
		/// </summary>
		/// <returns>height</returns>
		f32 Height()const;

		/// <summary>
		/// Gets the width of text line based on normalized screen scale
		/// </summary>
		/// <returns>the width of text</returns>
		f32 Width()const;
		char const* GetString()const;	// !< const getter for c style string
	private:
		f32 m_height, m_width;			// !< dimensions of text line based on normalized screen scale
		std::string line;				// !< string contained in line
	};
}
#endif
