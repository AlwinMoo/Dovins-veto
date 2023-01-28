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
	s32 m_boxWidth, m_boxHeight;
	s8 m_fontID;
public:
	TextArea(){}
	TextArea(s32 w, s32 h);
	~TextArea();
};
}
#endif 
