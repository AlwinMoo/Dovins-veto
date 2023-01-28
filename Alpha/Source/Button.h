#ifndef BUTTON_H
#define BUTTON_H
#include "AEEngine.h"
#include "TextArea.h"
namespace UI 
{
	struct Button
	{
		AEVec2 pos;
		AEVec2 size;
		s8 font;
		TextArea textArea;
		void(*callback)(void*);
		Button(){
		}
		~Button(){
		}
	};
}
#endif
