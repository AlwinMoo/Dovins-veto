#ifndef TEXTAREA_H
#define TEXTAREA_H
#include "AEEngine.h"
#include <string>
#include <vector>
namespace UI
{
class TextArea
{
private:
	std::vector<std::string> lines;
	s32 m_boxWidth, m_boxHeight;
	f32 m_scale;
	// Helper functions
	// Returns 0 if no break needed, otherwise returns size
	void GetPrintSize(std::string const& str, f32& width, f32& height);
	u32 ProcessLine(std::string const& line);
	// Trims line from the front to index and returns trimmed string
	std::string SplitLine(std::string& line, u32 index);
	void SetString(std::string& str);
public:
	// Constructors 
	TextArea();
	TextArea(s32 w, s32 h);
	TextArea(s32 w, s32 h, std::string const& str);

	void Draw();

	~TextArea();
};
}
#endif 
