#ifndef BUTTON_H
#define BUTTON_H
#include "AEEngine.h"
#include "TextArea.h"
#include "UI_Types.h"
namespace UI 
{
	struct Button
	{
		UI_TEX texID;
		UI_MESH meshID;
		
		AEVec2 pos;
		// world space pos
		AEVec2 wsPos;

		AEVec2 min, max;
		// World space boundaries
		AEVec2 wsMin, wsMax;
		AEVec2 scale;
		s8 font;
		TextArea textArea;
		void(*callback)();
		Button(){
		}
		~Button(){
		}
	};
}
#endif
