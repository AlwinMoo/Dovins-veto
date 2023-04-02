/******************************************************************************/
/*!
\file		pathfinder.cpp
\author		Alwin Moo
\par        email: moo.g\@digipen.edu
\date       April 02, 2023
\brief		Alwin (Primary 100%)

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
