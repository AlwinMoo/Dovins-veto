#ifndef UI_TEXTAREA_H
#define UI_TEXTAREA_H
#include "AEEngine.h"
#include <string>
#include <vector>
#include "UI_TextLineInfo.h"
#include "UI_Def.h"

namespace UI
{
class UI_TextArea
{
private:
	std::vector<UI_TextLineInfo> lines;
	f32 m_boxWidthN,	m_boxHeightN;		//!< NORMALIZED DIMENSIONS
	f32 m_boxWidth,		m_boxHeight;		//!< WORLD SPACE DIMENSIONS
	f32 m_scale;
	// Helper functions
	//!< Returns 0 if no break needed, otherwise returns size
	void GetPrintSize(std::string const& str, f32& width, f32& height);
	u32 ProcessLine(std::string const& line);
	//!< Trims line from the front to index and returns trimmed string
	std::string SplitLine(std::string& line, u32 end);
	void SetString(std::string& str);
public:

	// Constructors
	UI_TextArea();
	UI_TextArea(f32 w, f32 h,					f32 scale = 1.5f);
	UI_TextArea(f32 w, f32 h, std::string str,	f32 scale = 1.5f);

	void Draw(f32 screenX, f32 screenY, f32 r, f32 g, f32 b)const;
	void Draw(AEVec2 screenPosN, UI_Color color)const;

	// Getters
	f32 GetBoxWidth();
	f32 GetBoxHeight();
	f32 GetBoxWidthN();
	f32 GetBoxHeightN();
	~UI_TextArea();
};
}
#endif 
