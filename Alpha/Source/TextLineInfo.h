#ifndef TEXT_LINE_INFO_H
#define TEXT_LINE_INFO_H
#include <string>
#include "AEEngine.h"
namespace UI
{
	class TextLineInfo
	{
	public:
		TextLineInfo(std::string const& str, f32 scale);
		~TextLineInfo();
		f32 Height()const;
		f32 Width()const;
		char const* GetString()const;
	private:
		f32 m_height, m_width;
		std::string line;
	};
}
#endif
