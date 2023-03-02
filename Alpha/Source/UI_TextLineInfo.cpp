#include "UI_TextLineInfo.h"
namespace UI
{
	UI_TextLineInfo::UI_TextLineInfo(std::string const& str, f32 scale) : line(str)
	{
		AEGfxGetPrintSize(1, const_cast<s8*>(str.c_str()), scale, m_width, m_height);
	}

	UI_TextLineInfo::~UI_TextLineInfo()
	{
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
