#include "UI_Button.h"

void UI::UI_Button::CalculatePosN(AEVec2 screenDims)
{
	posN.x = { (posWS.x - scale.x * 0.5f) / screenDims.x * 2  };
	posN.y = { posWS.y / screenDims.y * 2  };
}
