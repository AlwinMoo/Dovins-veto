/******************************************************************************/
/*!
\file		UI_TextArea.h
\author		Alonzo Nalpon
\par        email: a.nalpon@digipen.edu
\date       January 28, 2023
\brief		Alonzo (Primary 100%)
			Contains declarations for a text area class

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/

#ifndef UI_TEXTAREA_H
#define UI_TEXTAREA_H
#include "AEEngine.h"
#include <string>
#include <vector>
#include "UI_TextLineInfo.h"
#include "UI_Def.h"

namespace UI
{
/// <summary>
/// Class that stores text areas with automatic wrapping functionality for words
/// </summary>
class UI_TextArea
{
private:
	std::vector<UI_TextLineInfo> lines;
	f32 m_boxWidthN,	m_boxHeightN;		//!< NORMALIZED DIMENSIONS
	f32 m_boxWidth,		m_boxHeight;		//!< WORLD SPACE DIMENSIONS
	f32 m_scale;
	UI::UI_Color m_color;
	// Helper functions
	//!< Returns 0 if no break needed, otherwise returns size
	void GetPrintSize(std::string const& str, f32& width, f32& height);
	u32 ProcessLine(std::string const& line);
	//!< Trims line from the front to index and returns trimmed string
	std::string SplitLine(std::string& line, u32 end);
	void SetString(std::string& str);
	/// <summary>
	/// Private ctor to set width, height, color and scale
	/// while calculating the box width in world
	/// </summary>
	/// <param name="w">width of text</param>
	/// <param name="h">height of text</param>
	/// <param name="color">color of text</param>
	/// <param name="scale">scale of text</param>
	UI_TextArea(f32 w, f32 h,					UI::UI_Color color  = {0.f,0.f,0.f,1.f},f32 scale = 1.5f);
public:

	UI_TextArea() = delete;
	
	/// <summary>
	/// Creates a text area with automatic wrapping based on dimensions of area
	/// </summary>
	/// <param name="w">width of text area</param>
	/// <param name="h">height of text area</param>
	/// <param name="str">string of text</param>
	/// <param name="color">color of text</param>
	/// <param name="scale">scale of the text</param>
	UI_TextArea(f32 w, f32 h, std::string str,	UI::UI_Color color  = {0.f,0.f,0.f,1.f},f32 scale = 1.5f);

	/// <summary>
	/// Draws text to a position with a custom color
	/// </summary>
	/// <param name="screenX">normalized coord x of text</param>
	/// <param name="screenY">normalized coord y of text</param>
	/// <param name="r">color red</param>
	/// <param name="g">color green</param>
	/// <param name="b">color blue</param>
	void Draw(f32 screenX, f32 screenY, f32 r, f32 g, f32 b)const;

	/// <summary>
	/// Draws text to a position with custom color
	/// </summary>
	/// <param name="screenPosN">normalized coords of text</param>
	/// <param name="color">color of text</param>
	void Draw(AEVec2 screenPosN, UI_Color color)const;

	/// <summary>
	/// Draws the text to a position with its own color stored
	/// </summary>
	/// <param name="screenPosN">normalized coord of text</param>
	void Draw(AEVec2 screenPosN)const;

	/// <summary>
	/// Gets box width in world
	/// </summary>
	/// <returns>box width</returns>
	f32 GetBoxWidth();

	/// <summary>
	/// Gets box height in world
	/// </summary>
	/// <returns>box height</returns>
	f32 GetBoxHeight();

	/// <summary>
	/// Gets box width in normalized size based on screen
	/// </summary>
	/// <returns>width of box</returns>
	f32 GetBoxWidthN();

	/// <summary>
	///  Gets box height in normalized size based on screen
	/// </summary>
	/// <returns>height of box</returns>
	f32 GetBoxHeightN();
};
}
#endif 
