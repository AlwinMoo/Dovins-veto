#ifndef UI_TEXT_AREA_TABLE_H
#define UI_TEXT_AREA_TABLE_H
#include "UI_TextAreaTable.h"
#include "UI_TextArea.h"
namespace UI
{
	struct UI_TextAreaTable 
	{
		UI::UI_TextArea endTurnHoverText;
		UI::UI_TextArea buildTowerHoverText;
		UI::UI_TextArea buildNexusHoverText;
		UI::UI_TextArea buildPlayerHoverText;
		UI::UI_TextArea buildWallHoverText;
		UI::UI_TextArea buildNexusPlacedHoverText;
		UI::UI_TextArea buildPlayerPlacedHoverText;
		UI::UI_TextArea eraseHoverText;
		UI::UI_TextArea elementTestText;

		UI_TextAreaTable();
		~UI_TextAreaTable() = default;
	};
}

#endif
