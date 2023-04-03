/******************************************************************************/
/*!
\file		UI_Button.cpp
\author 	Alonzo Nalpon
\par    	email: a.nalpon@digipen.edu
\date   	January 28, 2023
\brief		Alonzo (Primary 100%)
			This file contains the definitions of button class member function
			to be used with ui manager

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "UI_Button.h"

void UI::UI_Button::CalculatePosN(AEVec2 screenDims)
{
	posN.x = { (posWS.x - scale.x * 0.5f) / screenDims.x * 2  };
	posN.y = { posWS.y / screenDims.y * 2  };
}
