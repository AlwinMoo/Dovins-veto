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
