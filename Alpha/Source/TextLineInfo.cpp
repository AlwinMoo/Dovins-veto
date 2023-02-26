#include "TextLineInfo.h"
namespace UI
{
	TextLineInfo::TextLineInfo(std::string const& str, f32 scale) : line(str)
	{
		AEGfxGetPrintSize(1, const_cast<s8*>(str.c_str()), scale, m_width, m_height);
	}

	TextLineInfo::~TextLineInfo()
	{
	}

	f32 TextLineInfo::Height()const
	{
		return m_height;
	}

	f32 TextLineInfo::Width()const
	{
		return m_width;
	}
	char const* TextLineInfo::GetString()const
	{
		return line.c_str();
	}
}
