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

		AEVec2 min, max;		//!< Boundaries for screen space
		AEVec2 wsMin, wsMax;	//!< World space boundaries for rendering
		AEVec2 scale;			//!< scale of button
		s8 font; // TODO: REMOVE THIS WHEN IMPLEMENTED TEXT FULLY

		TextArea* buttonText;
		void(*callback)();
		TextArea* hoverText;
		bool bHovering;
		float alpha;			//!< Alpha for rendering: CURRENTLY NOT BEING USED
		
		Button() = default;
		~Button() = default;
	};
}
#endif
