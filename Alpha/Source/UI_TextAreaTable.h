#ifndef UI_TEXT_AREA_TABLE_H
#define UI_TEXT_AREA_TABLE_H
#include "UI_TextAreaTable.h"
#include "UI_TextArea.h"
namespace UI
{
	struct UI_TextAreaTable 
	{
		UI::UI_TextArea endTurnHoverText = { .3f, 1.f, "Ends The Build Phase. BE WARNED: YOU CANNOT BUILD DURING DEFENDING PHASE" };
		UI::UI_TextArea buildTowerHoverText = { .3f, 1.f, "Builds a tower. Automatically attacks enemies from range." };
		UI::UI_TextArea buildWallHoverText = { .3f, 1.f, "Builds a wall. Most enemies walk around them." };
		UI::UI_TextArea buildNexusHoverText = { .3f, 1.f, "Builds the nexus. Protect it with your life." };
		UI::UI_TextArea buildPlayerHoverText = { .3f, 1.f, "Place the player. Defend your base." };
		UI::UI_TextArea buildNexusPlacedHoverText = { .3f, 1.f, "Nexus already built. You only get one." };
		UI::UI_TextArea buildPlayerPlacedHoverText = { .3f, 1.f, "Player already placed. You only get one." };
		UI::UI_TextArea eraseHoverText = { .3f, 1.f, "Erase your building. No shame in mistakes." };
		// For skill tree
		

		UI_TextAreaTable();
		~UI_TextAreaTable() = default;
	};
}

#endif
