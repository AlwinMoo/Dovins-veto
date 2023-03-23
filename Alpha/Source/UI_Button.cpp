#include "UI_Button.h"

void UI::UI_Button::CalculatePosN(AEVec2 screenDims)
{
	//f32 const& mouseXN{ mouseX / winX * 2 - 1.f }, & mouseYN{ mouseY / winY * -2 + 1.f }; // CALCULATE NORMALIZED COORDINATES
	//AEVec2 pos{ static_cast<f32>(mouseX) + curr->hoverText->GetBoxWidth() * 0.25f,
	//					static_cast<f32>(-mouseY) - curr->hoverText->GetBoxHeight() * 0.25f };
	posN.x = { posWS.x / screenDims.x * 2  };
	posN.y = { posWS.y / screenDims.y * 2  };
}
