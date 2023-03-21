#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include "AEEngine.h"
#include "UI_TextArea.h"
#include "UI_Def.h"
namespace UI 
{
	struct UI_Button
	{
		UI_TEX texID;
		UI_MESH meshID;
		
		AEVec2 pos;
		// world space pos
		AEVec2 posWS;

		AEVec2 min, max;		//!< Boundaries for screen space
		AEVec2 minWS, maxWS;	//!< World space boundaries for rendering
		AEVec2 scale;			//!< scale of button
		s8 font; // TODO: REMOVE THIS WHEN IMPLEMENTED TEXT FULLY

		UI_TextArea* buttonText;
		void(*callback)(UI_Button*);
		UI_TextArea* hoverText;
		bool bHovering;
		float alpha;			//!< TODO: REMOVE
		
		UI_Button() = default;
		~UI_Button() = default;
	};
}
#endif
